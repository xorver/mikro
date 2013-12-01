CC=gcc
DEPS = basicWebcamFunctionality.h
OBJS = basicWebcamFunctionality.o webcamDemo.o

all:
	gcc monitoring.c -o monitoring

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

webcamDemo: basicWebcamFunctionality.o webcamDemo.o
	gcc -o $@ basicWebcamFunctionality.o webcamDemo.o

clean:
	rm -f webcamDemo
	rm -f monitoring
	rm -f *~
