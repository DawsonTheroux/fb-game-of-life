#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <linux/fb.h>

#include "frame_buffer.h"

int setup_frame_buffer_device(frame_buffer_dev_t *fb_dev)
{
	struct fb_var_screeninfo fb_var_info;
	fb_dev->fd = open("/dev/fb0", O_RDWR);
	if (!fb_dev->fd) {
		printf("failed to open /dev/fb0\n");
		return -1;
	}
	ioctl(fb_dev->fd, FBIOGET_VSCREENINFO, &fb_var_info);
	
	fb_dev->buffer_len = fb_var_info.xres * fb_var_info.yres * fb_var_info.bits_per_pixel / 8;
	fb_dev->xres = fb_var_info.xres;
	fb_dev->yres = fb_var_info.yres;
	fb_dev->bits_per_pixel= fb_var_info.bits_per_pixel;

	printf("buffer len: %u\n", fb_dev->buffer_len);

	fb_dev->buffer = (unsigned short *)mmap(NULL, 
			  	      fb_dev->buffer_len,
			  	      PROT_READ | PROT_WRITE, 
			  	      MAP_SHARED, 
			  	      fb_dev->fd, 
			  	      0x0);
	return 0;
}


int cleanup_frame_buffer_device(frame_buffer_dev_t *fb_dev)
{
	close(fb_dev->fd);
	munmap(fb_dev->buffer, fb_dev->buffer_len);
	return 0;
}


int draw_screen(frame_buffer_dev_t *fb_dev, frame_bitmap_t *frame_bitmap)
{
	unsigned int i, j;
	for(i=0; i<frame_bitmap->buffer_len; i++){
		for(j=0; j<sizeof(frame_bitmap->buffer) * 8; j++) {
			// fb_dev->buffer[(i * 8) + j] = 0xFFFF;
			if(((frame_bitmap->buffer[j] & (0x01 << j)) >> j) == 0x01){
				fb_dev->buffer[(i * 8) + j] = 0xFFFF;
			}else{
				fb_dev->buffer[(i * 8) + j] = 0x0000;
			}
		}
	}
	return 0;
}
