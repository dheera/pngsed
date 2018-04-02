all: pngsed pnggrep pngpixelstats

pngsed: pngsed.c
	gcc -o pngsed pngsed.c -lpng

pnggrep: pnggrep.c
	gcc -o pnggrep pnggrep.c -lpng

pngpixelstats: pngpixelstats.c
	gcc -o pngpixelstats pngpixelstats.c -lpng
