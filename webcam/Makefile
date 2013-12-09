DEPS = basicWebcamFunctionality.h email.h
OBJS = basicWebcamFunctionality.o webcamDemo.o email.o

all:
	gcc monitoring.c -o monitoring

%.o: %.c $(DEPS)
	gcc -c -o $@ $<

webcamDemo: basicWebcamFunctionality.o webcamDemo.o email.o
	gcc -o $@ basicWebcamFunctionality.o webcamDemo.o email.o

opencvDemo:
	g++ -ggdb -I/usr/include/opencv2 -o main test.cpp `pkg-config --libs opencv`


clean:
	rm -f webcamDemo
	rm -f monitoring
	rm -f *~
