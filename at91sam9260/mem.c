#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/mman.h>
#if 0
#define ISI_REG_BASE   (0xFFFC0000)
#define MAP_SIZE        0xFF
#else
#define ISI_REG_BASE   (0x20000000)
#define MAP_SIZE        0x4000000
#endif
static int dev_fd;
int main(int argc, char **argv)
{
	unsigned int reg;
	unsigned int value;
	if(argc == 1 || argc > 3) {
		printf("./anyu_mem 0X04\n");
		exit(1);
	}
	dev_fd = open("/dev/mem", O_RDWR | O_NDELAY);      
 
	if (dev_fd < 0)  
	{
		printf("open(/dev/mem) failed.");    
		return 0;
	}
	reg = strtoul(argv[1],NULL,NULL);
	unsigned char *map_base=(unsigned char * )mmap(NULL, 
						       MAP_SIZE, 
						       PROT_READ | PROT_WRITE, 
						       MAP_SHARED, 
						       dev_fd,
						       ISI_REG_BASE );
	printf("%08x \n", *(volatile unsigned int *)(map_base+reg)); //打印该寄存器地址的value
	if(argc == 3) {
		value = strtoul(argv[2],NULL,NULL);
		*(volatile unsigned int *)(map_base+reg) = value;
	}
	if(dev_fd)
		close(dev_fd);
	munmap(map_base,MAP_SIZE);//解除映射关系
	return 0;
}
