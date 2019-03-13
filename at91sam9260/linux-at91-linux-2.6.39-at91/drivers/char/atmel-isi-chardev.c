#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/sysctl.h>
#include <linux/wait.h>
#include <linux/bcd.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/dma-mapping.h>
#include <linux/module.h>


#define MAX_FRAME_LEN		(720UL*960UL)
#define ISI_MAGIC			0x20F05214

#define GET_PPFBD		0
#define GET_CDBA		1

struct isi_frame_buffer {
	u32 phy_current_addr;
	u32 phy_next_addr;
};

struct isi_priv_t {
	int isi_irq;
	void __iomem	 *reg_start;
	int isi_dev;
	int isi_major;
	int isi_minor;
	void *prev_buf;
	void *codec_buf;
	struct completion		complete;
	struct fasync_struct *fasync;
	u32 phy_codec_addr;
	dma_addr_t    phy_fb_addr;
	struct isi_frame_buffer *fb;
	struct class *isi_class;
	struct device *dev;
	u32			magic;
};

static struct isi_priv_t isi_priv;


static void isi_writel(struct isi_priv_t *isi, u32 reg, u32 val)
{
	writel(val, isi->reg_start + reg);
}
static u32 isi_readl(struct isi_priv_t *isi, u32 reg)
{
	return readl(isi->reg_start + reg);
}


static int atmel_isi_open(struct inode *inode, struct file *file)
{
	printk("atmel_isi_open\n");
	{
		file->private_data = &isi_priv;
		isi_priv.prev_buf = dma_alloc_coherent(isi_priv.dev,
											   MAX_FRAME_LEN,
											   &isi_priv.fb->phy_current_addr,
											   GFP_KERNEL);
		if(!isi_priv.prev_buf) {
			printk("dma alloc buf failed 01\n");
			return -ENOMEM;
		}
		printk("prev_buf %08X\n",isi_priv.fb->phy_current_addr);
		isi_priv.fb->phy_next_addr = isi_priv.fb->phy_current_addr;
		isi_priv.codec_buf = dma_alloc_coherent(isi_priv.dev,
											    MAX_FRAME_LEN,
											    &isi_priv.phy_codec_addr,
											    GFP_KERNEL);
		if(!isi_priv.codec_buf) {
			dma_free_coherent(isi_priv.dev,
						      MAX_FRAME_LEN,
						      isi_priv.prev_buf,
						      isi_priv.fb->phy_current_addr);
			printk("dma alloc buf failed 02\n");
			return -ENOMEM;
		}
		init_completion(&isi_priv.complete);
		/* soft reset */
		isi_writel(&isi_priv,0x00,0x01);
		msleep(10);
		isi_writel(&isi_priv,0x0C,0x01);   /* Enable SOF IRQ */
		isi_writel(&isi_priv,0x20,(479) | (639 << 16));  /* Setup ISI_PPFBD */
		isi_writel(&isi_priv,0x28,isi_priv.phy_fb_addr);  /* Setup ISI_PPFBD */
		isi_writel(&isi_priv,0x2C,isi_priv.phy_codec_addr);  /* Setup ISI_PPFBD */
		isi_writel(&isi_priv,0x00,0x8000); /* Enable module */
		/* Used for check memory overlap */
		isi_priv.magic = ISI_MAGIC;
	}
	return nonseekable_open(inode,file);
}

static int atmel_isi_release(struct inode *inode, struct file *file)
{
	struct isi_priv_t *priv;
	if(inode->i_rdev == isi_priv.isi_dev) {
		priv = file->private_data;
		if(priv->magic != ISI_MAGIC)
			return -ENODEV;
		/* disable Module */
		isi_writel(&isi_priv,0x00,0x02);
		isi_writel(&isi_priv,0x0C,0x00);  /* disable all interrupt */
		dma_free_coherent(isi_priv.dev,
					  MAX_FRAME_LEN,
					  isi_priv.codec_buf,
					  isi_priv.phy_codec_addr);
		dma_free_coherent(isi_priv.dev,
					  MAX_FRAME_LEN,
					  isi_priv.prev_buf,
					  isi_priv.fb->phy_current_addr);
	}
	return 0;
}

int isi_mmap(struct file *file, struct vm_area_struct *vma)
{
	size_t size = vma->vm_end - vma->vm_start;
	vma->vm_page_prot = pgprot_dmacoherent(vma->vm_page_prot);
//	if (!valid_mmap_phys_addr_range(vma->vm_pgoff, size))
//		return -EINVAL;
	/* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
	if (remap_pfn_range(vma,
			    vma->vm_start,
			    vma->vm_pgoff,
			    size,
			    vma->vm_page_prot)) {
		return -EAGAIN;
	}
	return 0;
}
int isi_fasync(int fd, struct file *filp, int on)
{
	struct isi_priv_t *priv = filp->private_data;
	if(priv != NULL && priv->magic == ISI_MAGIC) {
		return fasync_helper(fd,filp,on,&priv->fasync);
	}
	else {
		return 0;
	}
}

long isi_ioctl(struct file *file, unsigned int cmd , unsigned long arg)
{
	struct isi_priv_t *priv = file->private_data;
	if(priv != NULL && priv->magic == ISI_MAGIC) {
		switch(cmd) {
			case GET_PPFBD:
				__put_user(priv->fb->phy_current_addr, (__u32 __user *)arg);
			break;
			case GET_CDBA:
				__put_user(priv->phy_codec_addr, (__u32 __user *)arg);
			break;
			default:break;
		}
		return 0;
	}
	return -ENODEV;
}

static struct file_operations isi_ops = {
	.owner   = THIS_MODULE,
	.open    = atmel_isi_open,
	.release = atmel_isi_release,
	.unlocked_ioctl = isi_ioctl,
	.mmap = isi_mmap,
	.fasync = isi_fasync,
};


static irqreturn_t isi_interrupt(int irq, void *dev_id)
{
	struct isi_priv_t *priv = dev_id;
	if(irq == priv->isi_irq && priv->magic == ISI_MAGIC) {
		kill_fasync(&priv->fasync,SIGIO,POLL_IN);
		return IRQ_HANDLED;
	}
	else {
		return IRQ_NONE;
	}
}


static int isi_init(void)
{
	int ret;
	isi_priv.reg_start = ioremap(0xFFFC0000, 0x100);
	isi_priv.isi_irq = 22;
	isi_priv.isi_major = 123;
	/* Init IRQ */
    ret = request_irq(isi_priv.isi_irq,isi_interrupt,IRQF_SHARED,
                      "isi-irq",&isi_priv);
	if(ret < 0) {
		printk("ISI irq request failed\n");
		goto exit;
	}
	//create cdev
	ret = register_chrdev(isi_priv.isi_major, "isi", &isi_ops);
	if(ret != 0) {
		printk("ISI register chardev failed\n");
		goto irq_failed;
	}
	isi_priv.isi_class = class_create(THIS_MODULE, "isi_class");
	if (IS_ERR(isi_priv.isi_class)) {
		ret = -3;
		goto remove_cdev;
	}
	isi_priv.isi_minor = 0;
	isi_priv.isi_dev = MKDEV(isi_priv.isi_major,isi_priv.isi_minor);
	isi_priv.dev = device_create(isi_priv.isi_class,
								 NULL,isi_priv.isi_dev,&isi_priv,
								 "isi%d",isi_priv.isi_minor);
	if(!isi_priv.dev) {
		ret = -1;
		goto class_failed;
	}
	dma_set_coherent_mask(isi_priv.dev,DMA_BIT_MASK(32));
	isi_priv.fb = dma_alloc_coherent(isi_priv.dev,
					   				  sizeof(*isi_priv.fb),
					   				  &isi_priv.phy_fb_addr,
					   				  GFP_KERNEL);
	if(!isi_priv.fb) {
		ret = -2;
		goto class_failed;
	}
	printk("ISI chardev installed\n");
	return 0;
class_failed:
	class_destroy(isi_priv.isi_class);
remove_cdev:
	unregister_chrdev(isi_priv.isi_major,"isi");
irq_failed:
	free_irq(isi_priv.isi_irq,&isi_priv);
exit:
	return ret;
}

static void isi_exit(void)
{
	dma_free_coherent(isi_priv.dev,
					  sizeof(*isi_priv.fb),
					  isi_priv.fb,
					  isi_priv.phy_fb_addr);
	device_destroy(isi_priv.isi_class, isi_priv.dev);
	class_destroy(isi_priv.isi_class);
	unregister_chrdev(isi_priv.isi_major,"isi");
	free_irq(isi_priv.isi_irq,&isi_priv);
	printk("ISI chardev Removed\n");
}

module_init(isi_init);
module_exit(isi_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("lei.zhang");
MODULE_DESCRIPTION("isi test module\n") ;
MODULE_ALIAS("isi:0");


