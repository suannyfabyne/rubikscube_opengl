CC = g++

all:
	$(CC) main.cpp -std=c++0x -lGL -lglut -lGLU -I. -lSOIL -o cube


clean:
	rm main

