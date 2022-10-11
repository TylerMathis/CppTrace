all: build run clean_bin
gif: all build_gif

build:
	g++ -std=c++17 main.cpp -o main

run:
	./main

build_gif:
	python3 build_gif.py

clean_bin:
	rm main

clean_img:
	rm ./images/*.png
