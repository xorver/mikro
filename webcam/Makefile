DEPS = basicWebcamFunctionality.h email.h
OBJS = basicWebcamFunctionality.o webcamDemo.o email.o

all:
	gcc monitoring.c -o monitoring

%.o: %.c $(DEPS)
	gcc -I/usr/local/include/opencv2 -c -o $@ $< `pkg-config --libs opencv`

webcamDemo: basicWebcamFunctionality.o webcamDemo.o email.o
	gcc -I/usr/local/include/opencv2 -o $@ basicWebcamFunctionality.o webcamDemo.o email.o `pkg-config --libs opencv`

clean:
	rm -f webcamDemo
	rm -f monitoring
	rm -f *~
