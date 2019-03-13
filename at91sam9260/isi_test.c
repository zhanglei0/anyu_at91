#include <limits.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <signal.h>  

#define FRAME_LEN		(640 * 480)

static uint8_t sof = 0;
void isi_info(int signum)
{
	printf("isi signal\n");
	sof = 1;
}


int main(int argc,char *argv[])
{
	int fd;
	unsigned int cdba,ppfbd;
	uint8_t *p_cdba,*p_ppfbd;
	int Oflags;
	if(argc != 2) {
		printf("./isi_test /dev/isi0\n");
		exit(1);
	}
	fd = open(argv[1], O_RDWR, 0);
	if(fd <= 0) {
		printf("unable to open %s\n",argv[1]);
		perror("ISI:");
		exit(2);
	}
	ioctl(fd,1,&cdba);
	ioctl(fd,0,&ppfbd);
	printf("CDBA %08X PPFBD %08X\n",cdba,ppfbd);
	p_cdba  = mmap(NULL,FRAME_LEN,PROT_READ,MAP_SHARED,fd,cdba);
	p_ppfbd = mmap(NULL,FRAME_LEN,PROT_READ,MAP_SHARED,fd,ppfbd);
	if(!p_cdba || !p_ppfbd) {
		printf("mmap failed: %08X %08X\n",(uint32_t)p_cdba,(uint32_t)p_ppfbd);
		exit(3);
	}
	signal(SIGIO,isi_info);
	fcntl(fd, F_SETOWN, getpid());	
	Oflags = fcntl(fd, F_GETFL); 
	fcntl(fd, F_SETFL, Oflags | FASYNC);//设置设备程序支持异步通知
	while(1) {
		if(sof == 1) {
			
		}
		else 
		  usleep(1000);
	}
	close(fd);
	return 0;
}
