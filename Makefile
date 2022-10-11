all: build run clean_bin
gif: all build_gif

build:
	g++ -std=c++17 main.cpp -o main

run:
	./main

build_gif:
	python3 build_gif.py

clean_bin:
	echo "Cleaning binary" || rm main

clean_out:
	echo "Cleaning images" || rm ./images/*.png
	echo "Cleaning movie" || rm render.mov
