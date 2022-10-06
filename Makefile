all: build run clean

build:
	g++ -std=c++17 main.cpp -o main

run:
	./main

clean:
	rm main
