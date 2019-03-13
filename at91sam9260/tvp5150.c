#include<stdio.h>
#include <stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/types.h>
#include <time.h>
#include <string.h>

#include <malloc.h>

#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>





static int iic_read(int fd, unsigned char addr,unsigned char reg)
{
    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    outbuf = reg;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;
    messages[1].len   = sizeof(inbuf);
    messages[1].buf   = &inbuf;

    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(fd, I2C_RDWR, &packets) < 0)
    {
        perror("Unable to send data");
        return 1;
    }
    return inbuf;
}

static float SetRegValue(int fd,unsigned char dev,unsigned char cmd,
						  struct i2c_rdwr_ioctl_data *data)
{
	float y,n,value;
	short int y_i,n_i;
	unsigned short int reg;
	data->nmsgs = 2;
	data->msgs[0].len = 1;
	data->msgs[0].addr = dev;
	data->msgs[0].buf[0] = cmd ;
	data->msgs[0].flags = 0;
	
	data->msgs[1].len = 2;
	data->msgs[1].addr = dev;
	data->msgs[1].flags = I2C_M_RD;

	if(ioctl(fd,I2C_RDWR,(unsigned long)data) < 0) {
		perror("I2C");
		return 1;
	}
	memcpy(&reg,(data->msgs[1].buf),2);
	
	y_i = reg & 0x7FF;
	n_i = ((reg & 0xF800) >> 11) & 0x1F;
	if(reg & 0x8000) {
		n_i = n_i - 32;
	}
	else {
		y_i = reg;
		n_i = -13;
	}
	y = y_i;
	n = n_i;
//	printf("%02x %02x\n",\
			data->msgs[1].buf[1],\
			data->msgs[1].buf[0]);
	return value;
}

static char *device = "/dev/i2c-0";
static struct i2c_rdwr_ioctl_data data;

int main(int argc,char *argv[])
{
	int fd;
	char devname[32];
	//1.打开通用设备文件
	if(argc == 2){
		sprintf(devname,"/dev/i2c-%d",atoi(argv[1]));
	}
	else {
		strcpy(devname,device);
	}
	fd = open(devname,O_RDWR);
	if(fd < 0){
		perror("I2C");
		return 1;
	}
	data.msgs = (struct i2c_msg *)malloc(2*sizeof(struct i2c_msg));
	data.msgs[0].buf = malloc(16);
	data.msgs[1].buf = malloc(16);
	GetVoutValue(fd,0x42,0,&data);
	close(fd);
	return 0;
}
