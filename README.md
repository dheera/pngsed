# pngtools

## pngsed

Super-fast pixel-wise color replacement for PNG files. Useful for converting segmentation and other color annotations from one system to another.

````
./pngsed infile.png outfile.png ff0000:000000 00ff00:010101 0000ff:020202
````

converts red pixels to black rgb(0,0,0), green to rgb(1,1,1), blue to rgb(2,2,2).


## pngpixelstats

Given a PNG file, reports the number of pixels of the given colors. Useful for finding e.g. which segmentation annotations contain a particular class.

````
./pngpixelstats infile.png 000000 010101
infile.png 000000 947
infile.png 010101 0
````


## Compiling

````
sudo apt-get install libpng-dev
make
````

## Usage



