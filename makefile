all:
	gcc main.c src/*.c sulfur_libs/blt_libs/*.c -o build/main -lm -fno-builtin-printf
	build/main

win:
	if not exist build mkdir build
	if not exist "build/libs" mkdir "build/libs"
	gcc main.c src/*.c sulfur_libs/blt_libs/*.c -o build/main -lm -fno-builtin-printf
	make stdlibs_win

linux:
	mkdir -p build
	mkdir -p build/libs
	gcc main.c src/*.c sulfur_libs/blt_libs/*.c -o build/main -lm -fno-builtin-printf
	make stdlibs_linux

run:
	build/main

stdlibs_win:
	gcc -shared sulfur_libs/std_libs/sulfur_math.c build/API/memlib.c -o build/libs/math.dll -lm -fno-builtin-printf
	gcc -shared sulfur_libs/std_libs/graphic.c build/API/memlib.c -o build/libs/graphic.dll -lm -fno-builtin-printf -lgdi32
	gcc -shared sulfur_libs/std_libs/lilypad.c build/API/memlib.c -o build/libs/lilypad.dll -lm -fno-builtin-printf
	gcc -shared sulfur_libs/std_libs/poppy.c build/API/memlib.c -o build/libs/poppy.dll -lm -fno-builtin-printf 

stdlibs_linux:
	gcc -shared -fPIC sulfur_libs/std_libs/sulfur_math.c build/API/memlib.c -o build/libs/math.so -lm -fno-builtin-printf
	gcc -shared -fPIC sulfur_libs/std_libs/graphic.c build/API/memlib.c -o build/libs/graphic.so -lm -fno-builtin-printf -lX11
	gcc -shared -fPIC sulfur_libs/std_libs/lilypad.c build/API/memlib.c -o build/libs/lilypad.so -lm -fno-builtin-printf
	gcc -shared -fPIC sulfur_libs/std_libs/poppy.c build/API/memlib.c -o build/libs/poppy.dll -lm -fno-builtin-printf 
	
win_all:
	make win
	make run

linux_all:
	make linux
	make run

clean:
	rm -f main.exe
