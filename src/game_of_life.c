#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <linux/fb.h>

#include "game_of_life.h"

int calculate_next_state(unsigned char *old_state, unsigned char *new_state, unsigned int width, unsigned int height)
{
	printf("hello calcluate_next_state\n");
	return 0;

}
