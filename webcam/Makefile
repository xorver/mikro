DEPS = basicWebcamFunctionality.h email.h

all: basicWebcamFunctionality.o email.o
	gcc -I/usr/include/opencv2   main.c -o main basicWebcamFunctionality.o email.o `pkg-config --libs opencv`

%.o: %.c $(DEPS)
	gcc -I/usr/include/opencv2  -c -o $@ $< `pkg-config --libs opencv`

webcamDemo: basicWebcamFunctionality.o webcamDemo.o email.o
	gcc -o $@ basicWebcamFunctionality.o webcamDemo.o email.o

opencvDemo:
	g++ -ggdb -I/usr/include/opencv2 -o main test.cpp `pkg-config --libs opencv`


clean:
	rm -f webcamDemo main
	rm -f *~
	rm -f *.o
