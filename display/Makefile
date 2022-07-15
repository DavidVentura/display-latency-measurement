.PHONY: run

run: display
	./display

display: display.c Makefile
	gcc -O2 -o display display.c `sdl2-config --cflags --libs`

display.exe: display.c Makefile
	# http://libsdl.org/download-2.0.php "SDL2-devel mingw version"
	x86_64-w64-mingw32-gcc -O2 -o display.exe display.c -I ~/Downloads/SDL2-2.0.22/x86_64-w64-mingw32/include/SDL2 -L ~/Downloads/SDL2-2.0.22/x86_64-w64-mingw32/lib/ -lSDL2 -DSDL_MAIN_HANDLED
	cp ~/Downloads/SDL2-2.0.22/x86_64-w64-mingw32/bin/SDL2.dll .
