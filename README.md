# pngtools

## pngsed

Super-fast pixel-wise color replacement for PNG files. Useful for converting segmentation and other color annotations from one system to another.

````
./pngsed infile.png outfile.png ff0000:000000 00ff00:010101 0000ff:020202
````

converts red pixels to black rgb(0,0,0), green to rgb(1,1,1), blue to rgb(2,2,2).


## pnggrep

Greps a PNG file for color values and reports the number of instances. Does not output if color does not occur in the file. Useful for e.g. finding out which 

````
./pnggrep ff0000 infile.png
````

to check if there are #ff0000 pixels in the image.


## Compiling

````
sudo apt-get install libpng-dev
make
````

## Usage



