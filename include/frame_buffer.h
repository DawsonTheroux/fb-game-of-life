#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

typedef struct frame_buffer_dev_t {
	int fd; // The frame buffer file descriptor.
	struct fb_var_screeninfo fb_var_info;
	unsigned int buffer_len;
	unsigned int xres;
	unsigned int yres;
	unsigned int bits_per_pixel;
	unsigned short *buffer;
	unsigned short *current_buffer;
	unsigned int current_y_offset;
}frame_buffer_dev_t;

typedef struct frame_bitmap_t {
	unsigned char* buffer;
	unsigned int buffer_len;
}frame_bitmap_t;

int setup_frame_buffer_device(frame_buffer_dev_t *fb_dev);
int setup_offscreen_buffer_device(frame_buffer_dev_t *fb_dev, unsigned int width, unsigned int height);
int cleanup_frame_buffer_device(frame_buffer_dev_t *fb_dev);
int convert_bitmap_to_frame(frame_buffer_dev_t *fb_dev, unsigned char *frame_bitmap);
int display_frame(frame_buffer_dev_t *fb_dev);

#endif 
