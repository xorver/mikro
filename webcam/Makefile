DEPS = basicWebcamFunctionality.h email.h
OBJS = basicWebcamFunctionality.o webcamDemo.o email.o

all:
	gcc monitoring.c -o monitoring

%.o: %.c $(DEPS)
	gcc -c -o $@ $<

webcamDemo: basicWebcamFunctionality.o webcamDemo.o email.o
	gcc -o $@ basicWebcamFunctionality.o webcamDemo.o email.o

clean:
	rm -f webcamDemo
	rm -f monitoring
	rm -f *~
