CFLAGS = -Iinclude

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Target binary name
TARGET = $(BINDIR)/fb-game-of-life

# Find all source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
# Create a list of object files
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

# Link the binary
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build successful: $@"

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	@rm -rf $(OBJDIR) $(BINDIR)
	@echo "Cleaned up."

# Phony targets
.PHONY: all clean
