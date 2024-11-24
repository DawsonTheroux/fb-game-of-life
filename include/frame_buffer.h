#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

typedef struct frame_buffer_dev_t {
	int fd; // The frame buffer file descriptor.
	struct fb_var_screeninfo var_info;
	struct fb_fix_screeninfo fixed_info;
	unsigned int buffer_len;
	unsigned int xres;
	unsigned int yres;
	unsigned int bits_per_pixel;
	unsigned short *buffer;
}frame_buffer_dev_t;

typedef struct frame_bitmap_t {
	unsigned char* buffer;
	unsigned int buffer_len;
}frame_bitmap_t;

int setup_frame_buffer_device(frame_buffer_dev_t *fb_dev);
int cleanup_frame_buffer_device(frame_buffer_dev_t *fb_dev);
int draw_screen(frame_buffer_dev_t *fb_dev, frame_bitmap_t *frame_bitmap);

#endif 
