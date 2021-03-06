#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>

#include <linux/fb.h>

#define ERROR(x) printf("fbtest error in line %s:%d: %s\n", __FUNCTION__, __LINE__, strerror(errno));

#define FBCTL(cmd, arg)			\
	if(ioctl(fd, cmd, arg) == -1) {	\
		ERROR("ioctl failed");	\
		exit(1); }

#define FBCTL0(cmd)			\
	if(ioctl(fd, cmd) == -1) {	\
		ERROR("ioctl failed");	\
		exit(1); }

struct fb_var_screeninfo var;
struct fb_fix_screeninfo fix;

#define WIDTH 768
#define HEIGHT 1280

int open_fb(const char* dev)
{
	int fd = -1;
	fd = open(dev, O_RDWR);
	if(fd == -1)
	{
		printf("Error opening device %s : %s\n", dev, strerror(errno));
		exit(-1);
	}

	return fd;
}

static void draw_pixel(void *fbmem, int x, int y, unsigned color)
{
	if (var.bits_per_pixel == 16) {
		unsigned short c;
		unsigned r = (color >> 16) & 0xff;
		unsigned g = (color >> 8) & 0xff;
		unsigned b = (color >> 0) & 0xff;
		unsigned short *p;

		r = r * 32 / 256;
		g = g * 64 / 256;
		b = b * 32 / 256;

		c = (r << 11) | (g << 5) | (b << 0);

		fbmem += fix.line_length * y;

		p = fbmem;

		p += x;

		*p = c;
	} else {
		unsigned int *p;

		fbmem += fix.line_length * y;

		p = fbmem;

		p += x;

		*p = color;
	}
}

void fill_screen(void *fbmem)
{
	unsigned x, y;
	unsigned h = HEIGHT;
	unsigned w = WIDTH;
//	int color;

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {

#if 1 

draw_pixel(fbmem,x,y,0xffff0000);

draw_pixel(fbmem,x,y+HEIGHT,0xff00ff00);

draw_pixel(fbmem,x,y+HEIGHT*2,0xff0000ff);

#else
			if (x < 20 && y < 20)
				draw_pixel(fbmem, x, y, 0xffffff);
			else if (x < 20 && (y > 20 && y < h - 20))
				draw_pixel(fbmem, x, y, 0xff);
			else if (y < 20 && (x > 20 && x < w - 20))
				draw_pixel(fbmem, x, y, 0xff00);
			else if (x > w - 20 && (y > 20 && y < h - 20))
				draw_pixel(fbmem, x, y, 0xff0000);
			else if (y > h - 20 && (x > 20 && x < w - 20))
				draw_pixel(fbmem, x, y, 0xffff00);
			else if (x == 20 || x == w - 20 ||
					y == 20 || y == h - 20)
				draw_pixel(fbmem, x, y, 0xffffff);
			else if (x == y || w - x == h - y)
				draw_pixel(fbmem, x, y, 0xff00ff);
			else if (w - x == y || x == h - y)
				draw_pixel(fbmem, x, y, 0x00ffff);
			else if (x > 20 && y > 20 && x < w - 20 && y < h - 20) {
				int t = x * 3 / w;
				unsigned r = 0, g = 0, b = 0;
				unsigned c;
				if (var.bits_per_pixel == 16) {
					if (t == 0)
						b = (y % 32) * 256 / 32;
					else if (t == 1)
						g = (y % 64) * 256 / 64;
					else if (t == 2)
						r = (y % 32) * 256 / 32;
				} else {
					if (t == 0)
						b = (y % 256);
					else if (t == 1)
						g = (y % 256);
					else if (t == 2)
						r = (y % 256);
				}
				c = (r << 16) | (g << 8) | (b << 0);
				draw_pixel(fbmem, x, y, c);
			} else {
				draw_pixel(fbmem, x, y, 0);
			}
#endif
		}

	}
}


int main(int argc, char** argv)
{
	int fb_num;
	char str[64];
	int fd;

	if (argc == 2)
		sprintf(str,"%s",argv[1]);
	else
		sprintf(str,"/dev/fb0");

//	sprintf(str, "/dev/graphics/fb%d", fb_num);
	printf("opening %s\n", str);
	
	fd = open(str, O_RDWR);


	FBCTL(FBIOGET_VSCREENINFO, &var);
	FBCTL(FBIOGET_FSCREENINFO, &fix);
	
	// verify ioctl
	FBCTL(FBIOPUT_VSCREENINFO, &var);
	FBCTL(FBIOPUT_VSCREENINFO, &var);

	printf("res %d,%d virtual %d,%d, line_len %d\n",
			var.xres, var.yres,
			var.xres_virtual, var.yres_virtual,
			fix.line_length);
	printf("dim %d,%d\n", var.width, var.height);

	printf("Actual ling_len %d, bits_per_pixel %d\n", fix.line_length, var.bits_per_pixel);
	void* ptr = mmap(0, fix.smem_len ,
			PROT_WRITE | PROT_READ,
			MAP_SHARED, fd, 0);
	printf("smem_len: %d\n",fix.smem_len);

	if(ptr == MAP_FAILED) {
		perror("mmap failed");
		exit(1);
	}
	printf("current yoffset = %d\n", var.yoffset);

	fill_screen(ptr);

	while (1) {
	//display buffer 0
	var.yoffset = 0;
	FBCTL(FBIOPAN_DISPLAY, &var);
	usleep(500000);
	printf("current yoffset = %d\n", var.yoffset);
	//display buffer 1
	var.yoffset = HEIGHT;
	FBCTL(FBIOPAN_DISPLAY, &var);
	usleep(500000);
	printf("current yoffset = %d\n", var.yoffset);
	//display buffer 2
	var.yoffset = HEIGHT*2;
	FBCTL(FBIOPAN_DISPLAY, &var);
	usleep(500000);
	printf("current yoffset = %d\n", var.yoffset);
	}

	return 0;

}
