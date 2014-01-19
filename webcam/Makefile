CC = gcc
OBJECTS = basicWebcamFunctionality.o email.o JpegToAvi/jpegtoavi.o JpegToAvi/llist.o BitmapToJpeg/BitmapToJpeg.o BitmapToJpeg/BitBuffer.o BitmapToJpeg/Bitmap.o BitmapToJpeg/DibHeader.o BitmapToJpeg/DiscreteCosineTransform.o BitmapToJpeg/EncoderInput.o BitmapToJpeg/HuffmanEncoder.o BitmapToJpeg/JpegEncoder.o BitmapToJpeg/Pixel.o BitmapToJpeg/Tables.o

all: main clean

jpegtoavi.o: jpegtoavi.c avifmt.h byteswap.h
	$(CC) $(CFLAGS) -c $< -o $@ 
	
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ 	

main: $(OBJECTS)
	gcc -I/usr/include/opencv2   main.c -o main $(OBJECTS) `pkg-config --libs --cflags opencv` -ldl -lm
	
clean:
	rm -f *~
	rm -f *.o
	rm -f BitmapToJpeg/*.o
	rm -f JpegToAvi/*.o