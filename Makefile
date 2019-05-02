CC = g++

all:
	$(CC) main.cpp -std=c++0x -lGL -lglut -lGLU -o cube


clean:
	rm main

