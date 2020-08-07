
// emill105
// Ely Miller
// yli346
// yuntao Li
// ppm_io.c
// 601.220, Spring 2019
// Starter code for midterm project - feel free to edit/add to this file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ppm_io.h"

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check that fp is not NULL
  assert(fp); 

  Image * pic = malloc(sizeof(Image));

  char tag[4];
  fgets(tag, sizeof(tag), fp);
  if (tag[0] != 'P' || tag[1] != '6')
    { return NULL; }

  // looks for comment
  char c = fgetc(fp);
  //printf("%c\n", c);
  while (c == '#'){
    while (fgetc(fp) != '\n');
    c = fgetc(fp);
  }
  
  ungetc(c, fp);
  
  // retrieves image dimensions from header
  int num = fscanf(fp, " %d %d", &pic->cols, &pic->rows);
  if (num != 2){
    return NULL;
  }
  
  // retrieves color detail, checks if it's 255
  assert(fscanf(fp, " %d", &num) == 1);
  if (num != 255)
    {return NULL;}
  
  pic->data = malloc(pic->cols*pic->rows*3);  
  assert(pic->data);
  
  // skips past any potential nextlines
  c = fgetc(fp);
  while (c == '\n'){
    c = fgetc(fp);
  }

  ungetc(c, fp);
  
  // transfers file stream RGB data into structure format
  fread(pic->data, 3, pic->cols*pic->rows, fp);
   
  return pic;
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  assert(fp); 

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);
  
  // now write the pixel array
  int num_pixels_written = fwrite(im->data, 3, im->cols*im->rows, fp);
  fprintf(stdout, "%d\n", num_pixels_written);
  
  if (num_pixels_written != im->cols * im->rows) {
    fprintf(stderr, "Pixel data failed to write properly!\n");
    return -1;
  }
  
  free(im->data);
  
  return num_pixels_written;
}

