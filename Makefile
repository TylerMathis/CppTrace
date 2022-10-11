all: build run clean_bin

build:
	g++ -std=c++17 main.cpp -o main

run:
	./main

clean: clean_bin clean_img

clean_bin:
	rm main

clean_img:
	rm *.png
