CFLAGS = -g -Wall -I../source/u8glib  -DU8G_16BIT `sdl-config --cflags`

SRC = $(shell ls ../source/u8glib/*.c) $(shell ls *.c)

OBJ = $(SRC:.c=.o)

all: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl-config --libs` -o display

clean:	
	-rm $(OBJ)
