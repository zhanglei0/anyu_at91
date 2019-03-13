#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint8_t firmware[1024*1024*4] = {0};


int main(int argc,char *argv[])
{
	FILE *fp;
	memset(firmware,0xFF,sizeof(firmware));
	if(argc != 4) {
		printf("Wrong Command\n");
		exit(1);
	}
	char *bootsharp = argv[1];
	char *uboot = argv[2];
	char *kernel = argv[3];
	/* Open bootsharp */
	fp = fopen(bootsharp, "rb");
	fread(firmware,1,0x4000,fp);
	fclose(fp);
	
	fp = fopen(uboot, "rb");
	fread(firmware+0x8000,1,1024*256,fp);
	fclose(fp);
	
	fp = fopen(kernel, "rb");
	fread(firmware+0x48000,1,(1024*1024*4 - 0x48000),fp);
	fclose(fp);
	
	fp = fopen("output.bin", "wb+");
	fwrite(firmware,1,1024*1024*4,fp);
	fclose(fp);
	return 0;
}
