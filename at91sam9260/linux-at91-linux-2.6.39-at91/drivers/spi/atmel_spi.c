/*
 * Driver for Atmel AT32 and AT91 SPI Controllers
 *
 * Copyright (C) 2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

//#define DEBUG	

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>

#include <asm/io.h>
#include <mach/board.h>
#include <mach/gpio.h>
#include <mach/cpu.h>
#include <mach/at_hdmac.h>

#include "atmel_spi.h"

#define BUFFER_LEN		1024
/*
 * The core SPI transfer engine just talks to a register bank to set up
 * DMA transfers; transfer queue progress is driven by IRQs.  The clock
 * framework provides the base clock, subdivided for each spi_device.
 */
struct atmel_spi {
	spinlock_t		lock;
	unsigned long		flags;

	resource_size_t		phybase;
	void __iomem		*regs;
	int			irq;
	struct clk		*clk;
	struct platform_device	*pdev;

	u8			stopping;

	/* scratch buffer */
	void			*buffer;
};

/* Controller-specific per-slave state */
struct atmel_spi_device {
	unsigned int		npcs_pin;
	u32			csr;
};



#define BUFFER_SIZE		PAGE_SIZE
#define INVALID_DMA_ADDRESS	0xffffffff

/*
 * Version 2 of the SPI controller has
 *  - CR.LASTXFER
 *  - SPI_MR.DIV32 may become FDIV or must-be-zero (here: always zero)
 *  - SPI_SR.TXEMPTY, SPI_SR.NSSR (and corresponding irqs)
 *  - SPI_CSRx.CSAAT
 *  - SPI_CSRx.SBCR allows faster clocking
 *
 * We can determine the controller version by reading the VERSION
 * register, but I haven't checked that it exists on all chips, and
 * this is cheaper anyway.
 */
static bool atmel_spi_is_v2(void)
{
	return !cpu_is_at91rm9200();
}

static bool atmel_spi_v21x(struct atmel_spi *as)
{
	u32 v;

	v = SPI_BFEXT(VERS, spi_readl(as, VERSION));
	v &= 0x0ff0;
	return (v == 0x210);
}

/*
 * Earlier SPI controllers (e.g. on at91rm9200) have a design bug whereby
 * they assume that spi slave device state will not change on deselect, so
 * that automagic deselection is OK.  ("NPCSx rises if no data is to be
 * transmitted")  Not so!  Workaround uses nCSx pins as GPIOs; or newer
 * controllers have CSAAT and friends.
 *
 * Since the CSAAT functionality is a bit weird on newer controllers as
 * well, we use GPIO to control nCSx pins on all controllers, updating
 * MR.PCS to avoid confusing the controller.  Using GPIOs also lets us
 * support active-high chipselects despite the controller's belief that
 * only active-low devices/systems exists.
 *
 * However, at91rm9200 has a second erratum whereby nCS0 doesn't work
 * right when driven with GPIO.  ("Mode Fault does not allow more than one
 * Master on Chip Select 0.")  No workaround exists for that ... so for
 * nCS0 on that chip, we (a) don't use the GPIO, (b) can't support CS_HIGH,
 * and (c) will trigger that first erratum in some cases.
 *
 * TODO: Test if the atmel_spi_is_v2() branch below works on
 * AT91RM9200 if we use some other register than CSR0. However, don't
 * do this unconditionally since AP7000 has an errata where the BITS
 * field in CSR0 overrides all other CSRs.
 */

static void cs_activate(struct atmel_spi *as, struct spi_device *spi)
{
	struct atmel_spi_device *asd = spi->controller_state;
	unsigned active = spi->mode & SPI_CS_HIGH;
	u32 mr;

	mr = spi_readl(as, MR);
	if (atmel_spi_is_v2()) {
		/*
		 * Always use CSR0. This ensures that the clock
		 * switches to the correct idle polarity before we
		 * toggle the CS.
		 */
		spi_writel(as, CSR0, asd->csr);
		mr &= (SPI_BIT(MODFDIS) | SPI_BIT(MSTR) | SPI_BIT(WDRBT));
		mr |= SPI_BF(PCS, 0x0e);
		spi_writel(as, MR, mr);
		gpio_set_value(asd->npcs_pin, active);
	} else {
		u32 cpol = (spi->mode & SPI_CPOL) ? SPI_BIT(CPOL) : 0;
		int i;
		u32 csr;

		/* Make sure clock polarity is correct */
		for (i = 0; i < spi->master->num_chipselect; i++) {
			csr = spi_readl(as, CSR0 + 4 * i);
			if ((csr ^ cpol) & SPI_BIT(CPOL))
				spi_writel(as, CSR0 + 4 * i,
						csr ^ SPI_BIT(CPOL));
		}

		mr = SPI_BFINS(PCS, ~(1 << spi->chip_select), mr);
		if (spi->chip_select != 0)
			gpio_set_value(asd->npcs_pin, active);
		spi_writel(as, MR, mr);
	}

	dev_dbg(&spi->dev, "activate %u%s, mr %08x\n",
			asd->npcs_pin, active ? " (high)" : "",
			mr);
}

static void cs_deactivate(struct atmel_spi *as, struct spi_device *spi)
{
	struct atmel_spi_device *asd = spi->controller_state;
	unsigned active = spi->mode & SPI_CS_HIGH;
	u32 mr;

	/* only deactivate *this* device; sometimes transfers to
	 * another device may be active when this routine is called.
	 */
	mr = spi_readl(as, MR);
	if (~SPI_BFEXT(PCS, mr) & (1 << spi->chip_select)) {
		mr = SPI_BFINS(PCS, 0xf, mr);
		spi_writel(as, MR, mr);
	}

	dev_dbg(&spi->dev, "Deactivate %u%s, mr %08x\n",
			asd->npcs_pin, active ? " (low)" : "",
			mr);

	if (atmel_spi_is_v2() || spi->chip_select != 0)
		gpio_set_value(asd->npcs_pin, !active);
}

static void atmel_spi_lock(struct atmel_spi *as)
{
		spin_lock_irqsave(&as->lock, as->flags);
}

static void atmel_spi_unlock(struct atmel_spi *as)
{
		spin_unlock_irqrestore(&as->lock, as->flags);
}

static inline int atmel_spi_xfer_is_last(struct spi_message *msg,
					struct spi_transfer *xfer)
{
	return msg->transfers.prev == &xfer->transfer_list;
}

static inline int atmel_spi_xfer_can_be_chained(struct spi_transfer *xfer)
{
	return xfer->delay_usecs == 0 && !xfer->cs_change;
}

/*
 * plen is the transfer length of this xfer
*/
static void atmel_spi_xfer_data(struct spi_master *master,
				struct spi_transfer *xfer)
{
	struct atmel_spi	*as = spi_master_get_devdata(master);
	u32 i;
	u32 plen;
	u8 *tx,*rx;
	tx = xfer->tx_buf;
	rx = xfer->rx_buf;
	spi_writel(as,IDR,SPI_BIT(TXEMPTY));
	plen = xfer->len;
	for(i = 0; i < plen;i++){
		while(0 == (spi_readl(as,SR) & SPI_BIT(TXEMPTY)))
			cpu_relax();
		if(!tx)
			spi_writel(as, TDR, 0);
		else 
			spi_writel(as, TDR, *(tx + i));
		while(0 == (spi_readl(as,SR) & SPI_BIT(RDRF)))
			cpu_relax();
		if(rx != NULL)
			*(rx + i) = spi_readl(as,RDR) & 0xFFFF;
		else 
			spi_readl(as,RDR);
	}
}
static int atmel_spi_setup(struct spi_device *spi)
{
	struct atmel_spi	*as;
	struct atmel_spi_device	*asd;
	u32			scbr, csr;
	unsigned int		bits = spi->bits_per_word;
	unsigned long		bus_hz;
	unsigned int		npcs_pin;
	int			ret;

	as = spi_master_get_devdata(spi->master);

	if (as->stopping)
		return -ESHUTDOWN;

	if (spi->chip_select > spi->master->num_chipselect) {
		dev_dbg(&spi->dev,
				"setup: invalid chipselect %u (%u defined)\n",
				spi->chip_select, spi->master->num_chipselect);
		return -EINVAL;
	}

	if (bits < 8 || bits > 16) {
		dev_dbg(&spi->dev,
				"setup: invalid bits_per_word %u (8 to 16)\n",
				bits);
		return -EINVAL;
	}

	/* see notes above re chipselect */
	if (!atmel_spi_is_v2()
			&& spi->chip_select == 0
			&& (spi->mode & SPI_CS_HIGH)) {
		dev_dbg(&spi->dev, "setup: can't be active-high\n");
		return -EINVAL;
	}

	/* v1 chips start out at half the peripheral bus speed. */
	bus_hz = clk_get_rate(as->clk);
	if (!atmel_spi_is_v2())
		bus_hz /= 2;

	if (spi->max_speed_hz) {
		/*
		 * Calculate the lowest divider that satisfies the
		 * constraint, assuming div32/fdiv/mbz == 0.
		 */
		scbr = DIV_ROUND_UP(bus_hz, spi->max_speed_hz);

		/*
		 * If the resulting divider doesn't fit into the
		 * register bitfield, we can't satisfy the constraint.
		 */
		if (scbr >= (1 << SPI_SCBR_SIZE)) {
			dev_dbg(&spi->dev,
				"setup: %d Hz too slow, scbr %u; min %ld Hz\n",
				spi->max_speed_hz, scbr, bus_hz/255);
			return -EINVAL;
		}
	} else
		/* speed zero means "as slow as possible" */
		scbr = 0xff;

	csr = SPI_BF(SCBR, scbr) | SPI_BF(BITS, bits - 8);
	if (spi->mode & SPI_CPOL)
		csr |= SPI_BIT(CPOL);
	if (!(spi->mode & SPI_CPHA))
		csr |= SPI_BIT(NCPHA);

	/* DLYBS is mostly irrelevant since we manage chipselect using GPIOs.
	 *
	 * DLYBCT would add delays between words, slowing down transfers.
	 * It could potentially be useful to cope with DMA bottlenecks, but
	 * in those cases it's probably best to just use a lower bitrate.
	 */
	csr |= SPI_BF(DLYBS, 0);
	csr |= SPI_BF(DLYBCT, 0);

	/* chipselect must have been muxed as GPIO (e.g. in board setup) */
	npcs_pin = (unsigned int)spi->controller_data;
	asd = spi->controller_state;
	if (!asd) {
		asd = kzalloc(sizeof(struct atmel_spi_device), GFP_KERNEL);
		if (!asd)
			return -ENOMEM;

		ret = gpio_request(npcs_pin, dev_name(&spi->dev));
		if (ret) {
			kfree(asd);
			return ret;
		}

		asd->npcs_pin = npcs_pin;
		spi->controller_state = asd;
		gpio_direction_output(npcs_pin, !(spi->mode & SPI_CS_HIGH));
	} else {
		atmel_spi_lock(as);
		cs_deactivate(as, spi);
		atmel_spi_unlock(as);
	}

	asd->csr = csr;

	dev_dbg(&spi->dev,
		"setup: %lu Hz bpw %u mode 0x%x -> csr%d %08x\n",
		bus_hz / scbr, bits, spi->mode, spi->chip_select, csr);

	if (!atmel_spi_is_v2())
		spi_writel(as, CSR0 + 4 * spi->chip_select, csr);
	/* Interrupt mask: all interrupt can be triggered */
	spi_writel(as,IMR,0x3FF);
	return 0;
}
static void clean_spi_data(struct atmel_spi	*as)
{
	u32 status = spi_readl(as,SR);
	if(status & SPI_BIT(RDRF)) {
		spi_readl(as,RDR);
	}
}
static int atmel_spi_transfer(struct spi_device *spi, struct spi_message *msg)
{
	struct atmel_spi	*as;
	struct spi_transfer	*xfer;
	struct device		*controller = spi->master->dev.parent;
	u8			bits;
	struct atmel_spi_device	*asd;

	as = spi_master_get_devdata(spi->master);

	dev_dbg(controller, "new message %p submitted for %s\n",
			msg, dev_name(&spi->dev));
	
	
	if (unlikely(list_empty(&msg->transfers)))
		return -EINVAL;

	if (as->stopping)
		return -ESHUTDOWN;
	msg->actual_length = 0;
	msg->status = -EINPROGRESS;
	clean_spi_data(as);
	cs_activate(as, spi);
	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		if (!(xfer->tx_buf || xfer->rx_buf) && xfer->len) {
			dev_dbg(&spi->dev, "missing rx or tx buf\n");
			return -EINVAL;
		}

		if (xfer->bits_per_word) {
			asd = spi->controller_state;
			bits = (asd->csr >> 4) & 0xf;
			if (bits != xfer->bits_per_word - 8) {
				dev_dbg(&spi->dev, "you can't yet change "
					 "bits_per_word in transfers\n");
				return -ENOPROTOOPT;
			}
		}

		/* FIXME implement these protocol options!! */
		if (xfer->speed_hz) {
			dev_dbg(&spi->dev, "no protocol options yet\n");
			return -ENOPROTOOPT;
		}
		/**/
		
		atmel_spi_lock(as);
		atmel_spi_xfer_data(spi->master,xfer);
		atmel_spi_unlock(as);
		
		msg->actual_length += xfer->len;
	}
	cs_deactivate(as, spi);
	msg->status = 0;
	if (msg->complete)
		msg->complete(msg->context);
	return 0;
}

static void atmel_spi_cleanup(struct spi_device *spi)
{
	struct atmel_spi	*as = spi_master_get_devdata(spi->master);
	struct atmel_spi_device	*asd = spi->controller_state;
	unsigned		gpio = (unsigned) spi->controller_data;

	if (!asd)
		return;

	atmel_spi_lock(as);
	cs_deactivate(as, spi);
	atmel_spi_unlock(as);

	spi->controller_state = NULL;
	gpio_free(gpio);
	kfree(asd);
}

/*-------------------------------------------------------------------------*/

static int __init atmel_spi_probe(struct platform_device *pdev)
{
	struct resource		*regs;
	int			irq;
	struct clk		*clk;
	int			ret;
	u32			mr;
	struct spi_master	*master;
	struct atmel_spi	    *as;

	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!regs)
		return -ENXIO;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	clk = clk_get(&pdev->dev, "spi_clk");
	if (IS_ERR(clk))
		return PTR_ERR(clk);

	/* setup spi core then atmel-specific driver state */
	ret = -ENOMEM;
	master = spi_alloc_master(&pdev->dev, sizeof *as);
	if (!master)
		goto out_free;

	/* the spi->mode bits understood by this driver: */
	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;

	master->bus_num = pdev->id;
	master->num_chipselect = 4;
	master->setup = atmel_spi_setup;
	master->transfer = atmel_spi_transfer;
	master->cleanup = atmel_spi_cleanup;
	platform_set_drvdata(pdev, master);

	as = spi_master_get_devdata(master);

	/*
	 * Scratch buffer is used for throwaway rx and tx data.
	 * It's coherent to minimize dcache pollution.
	 */
	as->buffer = devm_kzalloc(&pdev->dev,BUFFER_SIZE,GFP_KERNEL);
	if (!as->buffer)
		goto out_free;

	spin_lock_init(&as->lock);
	as->pdev = pdev;
	as->regs = ioremap(regs->start, resource_size(regs));
	if (!as->regs)
		goto out_free_buffer;
	as->phybase = regs->start;
	as->irq = irq;
	as->clk = clk;
	/* Initialize the hardware */
	clk_enable(clk);
	spi_writel(as, CR, SPI_BIT(SWRST));
	spi_writel(as, CR, SPI_BIT(SWRST)); /* AT91SAM9263 Rev B workaround */
	mr = SPI_BIT(MSTR) | SPI_BIT(MODFDIS);
	if (atmel_spi_v21x(as))
		mr |= SPI_BIT(WDRBT); /* Prevent overrun errors */
	spi_writel(as, MR, mr);
	spi_writel(as, CR, SPI_BIT(SPIEN));

	/* go! */
	dev_info(&pdev->dev, "Atmel SPI Controller at 0x%08lx (irq %d)\n",
			(unsigned long)regs->start, irq);

	ret = spi_register_master(master);
	if (ret)
		goto out_reset_hw;

	return 0;
out_reset_hw:
	spi_writel(as, CR, SPI_BIT(SWRST));
	spi_writel(as, CR, SPI_BIT(SWRST)); /* AT91SAM9263 Rev B workaround */
	clk_disable(clk);
	free_irq(irq, master);
out_unmap_regs:
	iounmap(as->regs);
out_free_buffer:
	kfree(as->buffer);
out_free:
	clk_put(clk);
	spi_master_put(master);
	return ret;
}

static int __exit atmel_spi_remove(struct platform_device *pdev)
{
	struct spi_master	*master = platform_get_drvdata(pdev);
	struct atmel_spi	*as = spi_master_get_devdata(master);
//	struct spi_message	*msg;
//	struct spi_transfer	*xfer;

	/* reset the hardware and block queue progress */
	spin_lock_irq(&as->lock);
	as->stopping = 1;
	spi_writel(as, CR, SPI_BIT(SWRST));
	spi_writel(as, CR, SPI_BIT(SWRST)); /* AT91SAM9263 Rev B workaround */
	spi_readl(as, SR);
	spin_unlock_irq(&as->lock);

	/* Terminate remaining queued transfers */
	clk_disable(as->clk);
	clk_put(as->clk);
#ifdef SPI_INTERRUPT_ENABLED
	free_irq(as->irq, master);
#endif
	iounmap(as->regs);
	spi_unregister_master(master);
	return 0;
}
static struct platform_driver atmel_spi_driver = {
	.driver		= {
		.name	= "atmel_spi",
		.owner	= THIS_MODULE,
	},
	.remove		= __exit_p(atmel_spi_remove),
};

static int __init atmel_spi_init(void)
{
	return platform_driver_probe(&atmel_spi_driver, atmel_spi_probe);
}
module_init(atmel_spi_init);

static void __exit atmel_spi_exit(void)
{
	platform_driver_unregister(&atmel_spi_driver);
}
module_exit(atmel_spi_exit);

MODULE_DESCRIPTION("Atmel AT32/AT91 SPI Controller driver");
MODULE_AUTHOR("Haavard Skinnemoen <hskinnemoen@atmel.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:atmel_spi");
