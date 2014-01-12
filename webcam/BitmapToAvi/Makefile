VPATH =.
NAME = kloc
CFLAGS = 
CC = gcc
OBJECTS = buffer.o map.o head.o transform.o input.o huffman.o encoder.o const.o jpegtoavi.o llist.o

all: commands clean

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ 	
	

commands: $(OBJECTS) 
	$(CC) kloc.c -o $(NAME) $(OBJECTS) `pkg-config --libs --cflags opencv` -ldl -lm



clean:
	rm -f $(OBJECTS)
	rm -f *~

