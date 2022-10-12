all: build run clean_bin
gif: all build_mov

build:
	g++ -std=c++17 -O2 main.cpp -o main

run:
	./main

build_mov:
	python3 build_mov.py

clean_bin:
	echo "Cleaning binary"; rm main

clean_out:
	echo "Cleaning images"; rm ./images/*.png
	echo "Cleaning movie"; rm render.mov
