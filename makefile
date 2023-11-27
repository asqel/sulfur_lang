win:
	if not exist build mkdir build
	if not exist "build/libs" mkdir "build/libs"
	gcc main.c src/*.c sulfur_libs/blt_libs/*.c src/bytecode_maker/*.c -o build/sulfur -lm -fno-builtin-printf
	make stdlibs_win

linux:
	mkdir -p build
	mkdir -p build/libs
	gcc main.c src/*.c sulfur_libs/blt_libs/*.c src/bytecode_maker/*.c -o build/sulfur -lm -fno-builtin-printf -ldl

	make stdlibs_linux

profan:
	python3 profan_build.py

run:
	build/sulfur

stdlibs_win:
	gcc -shared sulfur_libs/std_libs/sulfur_math.c  -o build/libs/math.dll -lm -fno-builtin-printf
	gcc -shared sulfur_libs/std_libs/graphic/win_graphic.c sulfur_libs/std_libs/graphic/graphic_init.c  -o build/libs/graphic.dll -lm -fno-builtin-printf -lgdi32
	gcc -shared sulfur_libs/std_libs/lilypad.c  -o build/libs/lilypad.dll -lm -fno-builtin-printf
	gcc -shared sulfur_libs/std_libs/poppy.c  -o build/libs/poppy.dll -lm -fno-builtin-printf 
	gcc -shared sulfur_libs/std_libs/why.c  -o build/libs/why.dll -lm -fno-builtin-printf 

stdlibs_linux:
	gcc -shared -fPIC sulfur_libs/std_libs/sulfur_math.c  -o build/libs/math.so -lm -fno-builtin-printf
	gcc -shared -fPIC sulfur_libs/std_libs/graphic/linux_graphic.c sulfur_libs/std_libs/graphic/graphic_init.c  -o build/libs/graphic.so -lm -fno-builtin-printf -lX11
	gcc -shared -fPIC sulfur_libs/std_libs/lilypad.c  -o build/libs/lilypad.so -lm -fno-builtin-printf
	gcc -shared -fPIC sulfur_libs/std_libs/poppy.c  -o build/libs/poppy.so -lm -fno-builtin-printf 
	gcc -shared -fPIC sulfur_libs/std_libs/why.c  -o build/libs/why.so -lm -fno-builtin-printf 

win_all:
	make win
	make run

linux_all:
	make linux
	make run

linux_one_file:
	mkdir -p build
	mkdir -p build/libs
	gcc main.c src/*.c src/bytecode_maker/*.c sulfur_libs/blt_libs/*.c sulfur_libs/std_libs/*.c sulfur_libs/std_libs/graphic/*.c -o build/sulfur -lm -fno-builtin-printf -lX11 -D ONE_FILE

help:
	@echo win: build the interpreter and the built-in libs for windows
	@echo
	@echo linux: build the interpreter and the built-in libs for windows
	@echo 
	@echo run: run the interpreter with no arg
	@echo 
	@echo stdlibs_win: build std libs in .dll for windows
	@echo 
	@echo stdlibs_linux: build std libs in .so for linux
	@echo 	
	@echo win_all: build interpreter and std libs and run for windows
	@echo 
	@echo linux_all: build interpreter and std libs and run for linux
	@echo 
	@echo info: show this message
	@echo 
	@echo help: show this message
	@echo
	@echo linux_one_file : build sulfur with the std_libs in one file



fast:
	@make linux_one_file
	@echo start
	@./build/sulfur build/main.su
	@echo end

run_fast:
	./build/sulfur -m build/main.su
run_main:
	./build/sulfur build/main.su