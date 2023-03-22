all:
	gcc main.c src/*.c sulfur_libs/blt_libs/*.c -o build/main -lm -fno-builtin-printf
	build/main

compile:
	gcc main.c src/*.c sulfur_libs/blt_libs/*.c -o build/main -lm -fno-builtin-printf
run:
	build/main
	

clean:
	rm -f build/*.o
