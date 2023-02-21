all:
	gcc main.c src/*.c -o build/main
	

clean:
	rm -f build/*.o