CFLAGS=-Werror -Wall
FFTW=-L /usr/local/lib -lfftw3 -lm
CC=g++
INC=-I /usr/local/include

all: wavefile.o bitmap.o
	$(CC) $(INC) $(FFTW) Main.cpp WaveFile.o Bitmap.o -o main

wavefile.o:
	$(CC) $(CFLAGS) -c ./WaveFile.cpp

gif:
	convert -delay 3 -loop 0 ./Output/*.bmp ./result.gif

bitmap.o:
	$(CC) $(CFLAGS) -c ./Bitmap.cpp

clean:
	rm ./*.o ./main