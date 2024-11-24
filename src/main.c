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
#include "main.h"

int main()
{
	frame_buffer_dev_t fb_dev;
	frame_bitmap_t frame_bitmap;

	// Setup the frame buffer;
	if(setup_frame_buffer_device(&fb_dev)){
		printf("Failed to setup frame buffer device\n");
		return -1;
	}
	printf("bits per pixel: %d\n", fb_dev.bits_per_pixel);

	// Set frame bitmap.
	frame_bitmap.buffer_len = ((fb_dev.xres * fb_dev.yres) / 8);
	unsigned char game_state[frame_bitmap.buffer_len];
	memset(game_state, 0xAA, frame_bitmap.buffer_len);
	frame_bitmap.buffer = game_state;
	
	draw_screen(&fb_dev, &frame_bitmap);

	if(cleanup_frame_buffer_device(&fb_dev)){
		printf("Failed to cleanup frame buffer device\n");
		return -1;
	}
	return 0;
}
