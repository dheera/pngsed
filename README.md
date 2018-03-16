# pngsed

Super-fast pixel-wise color replacement for PNG files. Useful for converting segmentation and other color annotations from one system to another.

## Compiling

````
sudo apt-get install libpng-dev
make
````

## Usage

````
./pngsed infile.png outfile.png ff0000:000000 00ff00:010101 0000ff:020202
````

converts red pixels to black rgb(0,0,0), blue to rgb(1,1,1), green to rgb(2,2,2).

