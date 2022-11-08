ifeq ($(shell uname), Linux)
	OPEN := xdg-open
else
	OPEN := open
endif

all: cmake-build bin-build run open

cmake-build:
	cmake -S . -B cmake-build

bin-build:
	make -C cmake-build

run:
	./cmake-build/CppTrace

open:
	$(OPEN) ./images/*
