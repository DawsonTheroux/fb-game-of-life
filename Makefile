CC ?= gcc  # Default to gcc, overridden by Yocto's CC
CFLAGS ?= -Wall -Wextra -Iinclude
LDFLAGS ?=

# Source files and target binary
SRCS = src/main.c
OBJS = $(SRCS:.c=.o)
TARGET = fb-game-of-life

# Default rule
all: $(TARGET)

# Rule to link the binary
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Rule to compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean

