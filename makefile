all:
	gcc main.c libs/*.c -o main
	./main

SHARED_OBJS = $(patsubst %.c,%.so,$(wildcard dir/libs/*.c))

all: $(SHARED_OBJS)

build: all

%.so: %.c
	gcc -shared -o $@ $<

.PHONY: clean

clean:
	rm -f $(SHARED_OBJS)