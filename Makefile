build:
	g++ -std=c++17 -O2 main.cpp -o build/main

clean: clean_bin clean_out

clean_bin:
	echo "Cleaning binary"; rm main

clean_out:
	echo "Cleaning images"; rm ./images/*.png
	echo "Cleaning movie"; rm render.mov
