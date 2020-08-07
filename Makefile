CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g -lm


project: project.o ppm_io.o imageManip.o
	$(CC) -o project project.o ppm_io.o imageManip.o -lm

project.o: project.c
	$(CC) $(CFLAGS) -c project.c

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

imageManip.o: imageManip.c imageManip.h
	$(CC) $(CFLAGS) -c imageManip.c

clean:
	rm -f *.o project
