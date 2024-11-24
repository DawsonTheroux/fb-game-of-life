#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/sendfile.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#include "frame_buffer.h"

int setup_frame_buffer_device(frame_buffer_dev_t *fb_dev)
{
	fb_dev->fd = open("/dev/fb0", O_RDWR);
	if (fb_dev->fd < 0) {
		printf("failed to open /dev/fb0\n");
		return -1;
	}
	if(ioctl(fb_dev->fd, FBIOGET_VSCREENINFO, &fb_dev->fb_var_info)) {
		printf("Error reading screeninfo\n");
		return -1;
	}

	// Update the virtual resolution of the monitor for double buffering.
	fb_dev->fb_var_info.yres_virtual = fb_dev->fb_var_info.yres * 2;

	if(ioctl(fb_dev->fd, FBIOPUT_VSCREENINFO, &fb_dev->fb_var_info)) {
		printf("Error setting screen info with double virtual size\n");
		return -1;
	}
	
	fb_dev->buffer_len = fb_dev->fb_var_info.xres * fb_dev->fb_var_info.yres * fb_dev->fb_var_info.bits_per_pixel / 8;

	fb_dev->buffer = (unsigned short *)mmap(NULL, 
			  	      		fb_dev->buffer_len * 2, // Double frame buffer size for offscreen.
			  	      		PROT_READ | PROT_WRITE, 
			  	      		MAP_SHARED, 
			  	      		fb_dev->fd, 
			  	      		0x0);
	fb_dev->current_buffer = fb_dev->current_buffer;
	fb_dev->current_y_offset = 0;

	if(fb_dev->current_buffer == MAP_FAILED) {
		printf("Failed to mmap display buffer\n");
	}
	return 0;
}

/*
int setup_offscreen_buffer_device(frame_buffer_dev_t *fb_dev, unsigned int width, unsigned int height)
{
	fb_dev->buffer_len = width * height * sizeof(unsigned short);
	fb_dev->fb_var_info.xres = width;
	fb_dev->fb_var_info.yres = height;
	fb_dev->fd = memfd_create("frame_buf", MFD_NOEXEC_SEAL);
	if(fb_dev->fd < 0) {
		printf("Failed to create memfd for offscreen buffer\n");
		return -1;
	}
	ftruncate(fb_dev->fd, fb_dev->buffer_len);
	fb_dev->current_buffer = (unsigned short *)mmap(0,
						fb_dev -> buffer_len,
						PROT_READ | PROT_WRITE,
						MAP_SHARED,
						fb_dev->fd,
						0x0);
	if(fb_dev->current_buffer == MAP_FAILED) {
		printf("Failed to mmap offscreen buffer\n");
	}
	return 0;
}
*/


int cleanup_frame_buffer_device(frame_buffer_dev_t *fb_dev)
{
	close(fb_dev->fd);
	munmap(fb_dev->current_buffer, fb_dev->buffer_len);
	return 0;
}

int convert_bitmap_to_frame(frame_buffer_dev_t *fb_dev, unsigned char *frame_bitmap)
{
	unsigned int i, j;
	printf("Updating frame buffer with values: width %d, height %d\n", fb_dev->fb_var_info.xres, fb_dev->fb_var_info.yres);
	for(i=0; i<((fb_dev->fb_var_info.xres * fb_dev->fb_var_info.yres) / 8); i++){
		for(j=0; j<sizeof(frame_bitmap) * 8; j++) {
			if(((frame_bitmap[j] & (0x01 << j)) >> j) == 0x01){
				fb_dev->current_buffer[(i * 8) + j] = 0xFFFF;
			}else{
				fb_dev->current_buffer[(i * 8) + j] = 0x0000;
			}
		}
	}
	return 0;
}

int display_frame(frame_buffer_dev_t *fb_dev)
{
	// Check if the current buffer is the front.
	if(fb_dev->current_buffer == fb_dev->buffer) {
		fb_dev->fb_var_info.yoffset = fb_dev->fb_var_info.yres;
	} else {
		fb_dev->fb_var_info.yoffset = 0;
	}
    	if (ioctl(fb_dev->fd, FBIOPAN_DISPLAY, &fb_dev->fb_var_info)) {
        	perror("Error panning display");
        	munmap(fb_dev->buffer, fb_dev->buffer_len);
        	close(fb_dev->fd);
        	return 1;
    	}
	return 0;
}
