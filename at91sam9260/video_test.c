/*
 * v4l2_test.c
 * V4L2 video capture example
 * AUTHOT : WANGTISHENG
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h> /* getopt_long() */
#include <fcntl.h> /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

struct buffer {
        void *start;
        size_t length;
};

static char *dev_name;
static char file_name[20];
static int fd = -1;        //DEVICE NUMBER
struct buffer *buffers;
static unsigned int n_buffers ;
static unsigned frame_count = 1000;
FILE *fp;                                //FILE POINTOR

static void errno_exit(const char *s)
{
        fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
        exit(EXIT_FAILURE);
}

static int xioctl(int fh, int request, void *arg)
{
        int r;
        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);
        return r;
}
//处理函数
static void process_image(const void *p, int size,unsigned int no)
{
	if(no == 900) {
		fwrite(p,size, 1, fp);
	}
}

static int read_frame(FILE *fp,int no)
{
		int time = 0;
        struct v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
       /* Here to get one frame */
	   
        if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) 
                errno_exit("VIDIOC_DQBUF");
	process_image(buffers[buf.index].start, buf.bytesused,no);
        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
            errno_exit("VIDIOC_QBUF"); 
        return 1;
}

static void mainloop(void)
{
        unsigned int count;
        count = frame_count;
        while (count-- > 0) {
            printf("No.%d\n",frame_count - count);        //显示当前帧数目
            read_frame(fp,frame_count - count);
        }
        printf("\nREAD AND SAVE DONE!\n");
}

static void stop_capturing(void)
{
        enum v4l2_buf_type type;

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
            errno_exit("VIDIOC_STREAMOFF");
}

static void start_capturing(void)
{
        unsigned int i;
        enum v4l2_buf_type type;

        for (i = 0; i < n_buffers; ++i) {
                struct v4l2_buffer buf;

                CLEAR(buf);
                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;
                buf.index = i;

                if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                        errno_exit("VIDIOC_QBUF");
        }
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	printf("VIDIOC_STREAMON\n");
        if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
                errno_exit("VIDIOC_STREAMON");
}

static void uninit_device(void)
{
        unsigned int i;

        for (i = 0; i < n_buffers; ++i)
                if (-1 == munmap(buffers[i].start, buffers[i].length))
                        errno_exit("munmap");

        free(buffers);
}



static void init_mmap(void)
{
        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count = 1;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s does not support memory mapping\n", 
                            dev_name);
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_REQBUFS");
                }
        }

        if (req.count < 1) {
                fprintf(stderr, "Insufficient buffer memory on %s\n",
                         dev_name);
                exit(EXIT_FAILURE);
        }

        buffers = calloc(req.count, sizeof(*buffers));

        if (!buffers) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
                struct v4l2_buffer buf;

                CLEAR(buf);

                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;
                buf.index = n_buffers;

                if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
                        errno_exit("VIDIOC_QUERYBUF");

                buffers[n_buffers].length = buf.length;
                buffers[n_buffers].start =
                        mmap(NULL /* start anywhere */,
                              buf.length,
                              PROT_READ | PROT_WRITE /* required */,
                              MAP_SHARED /* recommended */,
                              fd, buf.m.offset);

                if (MAP_FAILED == buffers[n_buffers].start)
                        errno_exit("mmap");
        }
}


static void init_device(void)
{
        struct v4l2_capability cap; //查询设备功能、性能
        struct v4l2_format fmt;//即视频数据格式类型结构体

        if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {        //查询视频设备的功能,query capture查询捕捉，成功返回0
                if (EINVAL == errno) {
                        fprintf(stderr, "%s is no V4L2 device\n",
                                 dev_name);
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_QUERYCAP");
                }
        }

        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {        //判断是否支持视频捕获功能V4L2_CAP_VIDEO_CAPTURE
                fprintf(stderr, "%s is no video capture device\n",
                         dev_name);
                exit(EXIT_FAILURE);
        }

        if (!(cap.capabilities & V4L2_CAP_STREAMING)) {        //判断是否支持视频捕获功能V4L2_CAP_STREAMING
                fprintf(stderr, "%s does not support streaming i/o\n",
                    dev_name);
                exit(EXIT_FAILURE);
        }


        CLEAR(fmt); 
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;        // 数据流类型，必须永远是V4L2_BUF_TYPE_VIDEO_CAPTURE
        fmt.fmt.pix.width = 320;    // 宽，必须是16 的倍数
        fmt.fmt.pix.height = 480;    // 高，必须是16 的倍数
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;    //视频数据存储类型，例如是YUV 4 ：2 ：2 还是RGB
                //YUV444的FourCC称为 AYUV；YUV422(或YUV420)的FourcCC 按字节序分为YUY2和UYVY (或IMC1和IMC2)
        fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;        //汇报当前的field顺序
                //V4L2_FIELD_INTERLACED:Images包含top和bottom field, 隔行交替，场序依赖于当前video的标准。NTSC首先传输bottom field, PAL则先传输top field。

        if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))        //设置视频设备的视频格式
                errno_exit("VIDIOC_S_FMT");  
        init_mmap();
}

static void close_device(void)
{
        if (-1 == close(fd))
                errno_exit("close");

        fd = -1;
}

static void open_device(void)        //函数前加static表示内部函数，仅限本文件调用
{
        //加上O_NONBLOCK会出现如下错误
        //VIDIOC_DQBUF error 11, Resource temporarily unavailable
        fd = open(dev_name, O_RDWR /* required */ /*| O_NONBLOCK*/, 0);        //O_RDWR|O_NONBLOCK：使用非堵塞可读可写模式打开，现使用的是堵塞模式

        if (-1 == fd) {
                fprintf(stderr, "Cannot open '%s': %d, %s\n",
                         dev_name, errno, strerror(errno));        //int fprintf(FILE *stream,const char *format,...);
                exit(1);
        }
}


int main(int argc, char **argv)
{
	dev_name = "/dev/video0";

	if(argc != 2)
	{
		printf("usage :%s filename\n", argv[0]);
		exit(0);
	}
	if ((fp = fopen(argv[1], "w")) == NULL) { 
		perror("Creat file failed"); 
		exit(0); 
	}
	strcpy(file_name,argv[1]);
	open_device();//打开设备
	init_device();//查询，设置视频格式
	start_capturing();//请求分配缓冲，获取内核空间，
	printf("start capturing\n");
	mainloop();                //主要处理均在该函数中实现
	stop_capturing();
	uninit_device();
	close_device();
	//fprintf(stderr, "\n");
	return 0;
}
