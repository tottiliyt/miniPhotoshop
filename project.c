#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ppm_io.h"
#include "imageManip.h"

//yli346
//yuntao Li
//emill105
//Ely Miller
//project.c

int checkArgs(int argc, char* argv[])
{
  if (argc < 4){
    return 1;
  }
  
  int len1 = strlen(argv[1]);
  int len2 = strlen(argv[2]);
  if (len1 < 5 || len2 < 5){
    return 1;
  }

  // ensures input/output filenames end in ".ppm"
  if (argv[1][len1-4] == '.' && argv[1][len1-3] == 'p'
   && argv[1][len1-2] == 'p' && argv[1][len1-1] == 'm'){
  }
  else{
    printf("Input file (second argument) not recognized as a .ppm file\n");
    return 1;
  }
  if (argv[2][len2-4] == '.' && argv[2][len2-3] == 'p'
   && argv[2][len2-2] == 'p' && argv[2][len2-1] == 'm'){
  }
  else{
    printf("Output file (second argument) not recognized as a .ppm file\n");
    return 1;
  }
  
  return 0;
}

int run_func(char *argv[], Image *im)
{
  // selects the function based on user input
  char *func = argv[3];
  int didFunc = 0;
  
  char *current = "exposure";
  if (!strcmp(func, current))
  {
    printf("start exposure");
    int val = atof(argv[4]);
    if (val >= -3 && val <= 3)
      { exposure(im, val); }
    else {
      fprintf(stderr, "EV outside range (-3 < ev < 3)\n");
      return 6;
    }
    didFunc = 1;
  }

  current = "blend";
  if (!strcmp(func, current))
  {
      printf("start blend");
    FILE *input = fopen(argv[4], "rb");
    Image *im2 = read_ppm(input);
    if (!im2){
      fprintf(stderr, "Read failed\n");
      return 3;
    }
    double val = atof(argv[5]);
    if (val < 0 || val > 1){
      fprintf(stderr, "Alpha value out of range (0 < a < 1)\n");
      return 6;
    }
    blend(im, im2, val);
    free(im2->data);
    free(im2);
    fclose(input);
    didFunc = 1;
  }

  current = "zoom_in";
  if (!strcmp(func, current)){
      printf("start zoomin");
    zoom_in(im);
    didFunc = 1;
  }

  current = "zoom_out";
  if (!strcmp(func, current)){
      printf("start zoomout");
    zoom_out(im);
    didFunc = 1;
  }

  current = "pointilism";
  if (!strcmp(func, current)){
      printf("start pointilism");
    pointilism(im);
    didFunc = 1;
  }

  current = "swirl";
  if (!strcmp(func, current))
  {
      printf("start swirl");
    int x = atof(argv[4]);
    int y = atof(argv[5]);
    int s = atof(argv[6]);
    if (x < 0 || x > im->cols || y < 0 || y > im->rows){
      fprintf(stderr, "Center coordinates are not within the image boundaries\n");
      return 6;
    }
    if (s < 0){
      fprintf(stderr, "Strength can only be a positive integer\n");
      return 6;
    }
    swirl(im, x, y, s);
    didFunc = 1;
  }

  current = "blur";
  if (!strcmp(func, current))
  {
      printf("start blur");
    int sigma = atof(argv[4]);
    blur(im, sigma);
    didFunc = 1;
  }

  if (!didFunc){
    fprintf(stderr, "Operation entered is not recognized\n");
    return 4;
  }
  
  // returns 0 if function runs successfully
  return 0;
}

int main(int argc, char* argv[])
{
  int err_check = checkArgs(argc, argv);
  if (err_check){
    return err_check;
  }

  // reads ppm file to Image struct
  FILE *input = fopen(argv[1], "rb");
  Image *im = read_ppm(input);
  if (!im){
    printf("Read failed\n");
    return 2;
  }

  // runs image data through function
  err_check = run_func(argv, im);
  if (err_check){
    return err_check;
  }
  
  // writes Image struct to ppm file
  FILE *fp = fopen(argv[2], "wb");
  assert(fp);
  err_check = write_ppm(fp, im);
  if (err_check < 0){
    return 7;
  }
  free(im);
  fclose(input);
  fclose(fp);
  
  return 0;
}

