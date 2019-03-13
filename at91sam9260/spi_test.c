/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */
 
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h> 
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
 
static void pabort(const char *s)
{
	perror(s);
	abort();
}
 
static const char *device = "/dev/spidev1.1";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;
uint8_t tx[32] = {0x11,0x22,0x33,0x44,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
uint8_t rx[ARRAY_SIZE(tx)] = {0, };
static void transfer(int fd)
{
	int ret;
	memset(rx,0,sizeof(rx));
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx),
		.delay_usecs = delay,
		.bits_per_word = bits,
	};
 	
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
	printf("tx %p rx %p\n",tx,rx);
	for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
                printf("%02X ",tx[ret]);
		printf("%02X ", rx[ret]);
		printf("\n");
	}
	printf("\n");
}
 
static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.1)\n"
	     "  -s --speed    max speed (Hz)\n"
	     "  -d --delay    delay (usec)\n"
	     "  -b --bpw      bits per word \n"
	     "  -l --loop     loopback\n"
	     "  -H --cpha     clock phase\n"
	     "  -O --cpol     clock polarity\n"
	     "  -L --lsb      least significant bit first\n"
	     "  -C --cs-high  chip select active high\n"
	     "  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}
 
static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 's' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ NULL, 0, 0, 0 },
		};
		int c;
 
		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);
 
		if (c == -1)
			break;
 
		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 's':
			speed = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'b':
			bits = atoi(optarg);
			break;
		case 'l':
			mode |= SPI_LOOP;
			break;
		case 'H':
			mode |= SPI_CPHA;
			break;
		case 'O':
			mode |= SPI_CPOL;
			break;
		case 'L':
			mode |= SPI_LSB_FIRST;
			break;
		case 'C':
			mode |= SPI_CS_HIGH;
			break;
		case '3':
			mode |= SPI_3WIRE;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}
 
int main(int argc, char *argv[])
{
	int err = 0;
	unsigned char cmd = 8;
	unsigned int mode = 0;
	int handle_spi1dev = 0;
	int ret = 0;
	handle_spi1dev = open("/dev/spidev1.0",O_RDWR);
 
	if(handle_spi1dev < 0)
	{	
		printf("/dev/spidev1.0 error \n");
	}	
	mode = SPI_CPHA;
	err = ioctl(handle_spi1dev, SPI_IOC_WR_BITS_PER_WORD,&cmd);//change spi to 32bits mode
	if (err == -1)
		printf("can't set spi mode\n");
 
	err = ioctl(handle_spi1dev, SPI_IOC_WR_MODE, &mode);
	if (err == -1)
		printf("can't set spi mode\n");
	transfer(handle_spi1dev);
	close(handle_spi1dev);
 
	return ret;
}

