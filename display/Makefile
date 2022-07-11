.PHONY: run

run: display
	./display

display: display.c Makefile
	gcc -O2 -o display display.c `sdl2-config --cflags --libs`
