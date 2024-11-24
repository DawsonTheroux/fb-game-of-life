#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <linux/fb.h>

#include "frame_buffer.h"
#include "game_of_life.h"
#include "main.h"

#define NUM_GAME_STATES 2

/* TODO */
/* - Add multiple frame buffers */
/* - Add DMA for frame buffers  */

int main()
{
	frame_buffer_dev_t fb_dev;
	// frame_buffer_dev_t offscreen_fb_dev;
	// unsigned short *offscreen_buffer;
	unsigned char *game_states[NUM_GAME_STATES];
	unsigned int frame_len;

	// Setup the frame buffer;
	if(setup_frame_buffer_device(&fb_dev)) {
		printf("Failed to setup frame buffer device\n");
		return -1;
	}
	printf("bits per pixel: %d\n", fb_dev.bits_per_pixel);

	frame_len = ((fb_dev.xres * fb_dev.yres) / 8);
	/*
	if(setup_offscreen_buffer_device(&offscreen_fb_dev, fb_dev.xres, fb_dev.yres)) {
		printf("Failed to setup offscreen frame buffer device\n");
		return -1;
	}
	*/
	
	// offscreen_buffer = (unsigned short *)malloc(sizeof(unsigned short) * frame_len);
	// Alocate the game states.
	for(int i=0; i<NUM_GAME_STATES; i++) {
		game_states[i] = (unsigned char *)malloc(sizeof(unsigned char) * frame_len);
		if(!game_states[i]){
			printf("Failed to allocate game_state at %d\n", i);
			return 1;
		}
	}

	calculate_next_state(game_states[0], game_states[1], fb_dev.xres, fb_dev.yres);
	convert_bitmap_to_frame(&fb_dev, game_states[0]);
	// convert_bitmap_to_frame(&offscreen_fb_dev, game_states[0]);
	// convert_bitmap_to_frame(&fb_dev, game_states[0]);
	display_frame(&fb_dev);

	if(cleanup_frame_buffer_device(&fb_dev)){
		printf("Failed to cleanup frame buffer device\n");
		return -1;
	}
	return 0;
}
