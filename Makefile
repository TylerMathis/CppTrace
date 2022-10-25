all: build run

build:
	cmake -S . -B cmake-build

run:
	make -C cmake-build
	./cmake-build/raytracer
