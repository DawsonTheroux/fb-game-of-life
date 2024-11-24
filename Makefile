CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
LDFLAGS =
SRC = src/main.c
OBJ = $(SRC:.c=.o)
BIN = fb-game-of-life 

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)
