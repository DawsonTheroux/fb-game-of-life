CC = gcc
CFLAGS = -I../include -Wall -Wextra
LDFLAGS =
SRC = main.c
OBJ = $(SRC:.c=.o)
BIN = fb-game-of-life 

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)
