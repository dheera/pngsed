#include <stdlib.h>
#include <stdio.h>
#include <png.h>

int width, height;
unsigned int search_int;
unsigned int search_count = 0;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers;

void read_png_file(char *filename) {
  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) abort();

  png_infop info = png_create_info_struct(png);
  if(!info) abort();

  if(setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  width      = png_get_image_width(png, info);
  height     = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  if(bit_depth == 16) {
    printf("warning: squashing 16-bit image to 8-bit\n");
    png_set_strip_16(png);
  }

  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }

  png_read_image(png, row_pointers);

  fclose(fp);
}

void process_png_file() {
  unsigned int* color;
  for(int y = 0; y < height; y++) {
    png_bytep row = row_pointers[y];
    for(int x = 0; x < width; x++) {
      png_bytep px = &(row[x * 4]);
      color = (unsigned int*)(&row[x*4]);
      if(search_int == ((*color) & 0xFFFFFF)) {
        search_count++;
      }
    }
  }
}

int main(int argc, char *argv[]) {
  unsigned int i;
  char* search_str;

  if(argc != 3) {
    printf("Returns filename if a PNG file contains a pixel with specified color.\n");
    printf("Usage: \n");
    printf("    %s ff0000 infile.png\n", argv[0]);
    return 1;
  }

  search_int = (int)strtol(argv[1], NULL, 16);
  // byte swap for BGR->RGB
  search_int = ((search_int<<16)&0xff0000) | 
           ((search_int<<8)&0x00ff00) |
           (search_int>>16);

  read_png_file(argv[2]);
  process_png_file();
//  if(search_count > 0) {
    printf("%s %d\n",argv[2],search_count);
//  }
  return 0;
}
