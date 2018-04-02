#include <stdlib.h>
#include <stdio.h>
#include <png.h>

int width, height;
unsigned int search_int[256];
unsigned int search_int_len = 0;
unsigned int search_count[256];
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
  int i;
  for(int y = 0; y < height; y++) {
    png_bytep row = row_pointers[y];
    for(int x = 0; x < width; x++) {
      png_bytep px = &(row[x * 4]);
      color = (unsigned int*)(&row[x*4]);
      for(i=0;i<search_int_len;i++) {
        if(search_int[i] == ((*color) & 0xFFFFFF)) {
          search_count[i]++;
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {
  unsigned int i;
  char* search_str;

  if(argc <= 2) {
    printf("Returns statistics on the number of pixels of infile.png that contain pixels of given RGB colors.\n");
    printf("Usage: \n");
    printf("    %s infile.png ff0000 00ff00 0000ff ...\n", argv[0]);
    return 1;
  }

  search_int_len = argc - 2;
  for(i=0;i<argc-2;i++) {
    search_int[i] = (int)strtol(argv[i+2], NULL, 16);
    // byte swap for BGR->RGB
    search_int[i] = ((search_int[i]<<16)&0xff0000) | 
             ((search_int[i]<<8)&0x00ff00) |
             (search_int[i]>>16);
    search_count[i] = 0;
  }
  read_png_file(argv[1]);
  process_png_file();
  for(i=0;i<search_int_len;i++) {
    printf("%s %s %d\n",argv[1],argv[2+i],search_count[i]);
  }
  return 0;
}
