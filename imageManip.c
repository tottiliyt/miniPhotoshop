#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "imageManip.h"
#include "ppm_io.h"


//emill105
//Ely Miller
//yli346
//yuntao Li


void exposure(Image *im, double ev){
  int cols = im->cols;
  int rows = im->rows;
    Pixel* expo = malloc(cols * rows * 3);

    for (int i = 0; i < cols * rows; i++)
    {
        Pixel* temp = expo + i;
        temp->r = 0;
        temp->g = 0;
        temp->b = 0;
    }
    int count = 0;

    for (int i = 0; i < rows*cols; i++) {
            Pixel* temp = expo + count;
            Pixel* im1p = im->data +count;
            
            if ((im1p->r * pow(2, ev))> 255.0) {
                temp->r = 255;
            }
            else {
                temp->r = im1p->r * pow(2, ev);
            }
            if (im1p->g * pow(2, ev) > 255.0) {
                temp->g = 255;
            }
            else {
                temp->g = im1p->g * pow(2, ev);
            }
            if ( im1p->b * pow(2, ev)> 255.0) {
                temp->b = 255;
            }
            else {
                temp->b =  im1p->b * pow(2, ev);;
            }
            count++;
        
    }
    Pixel* old = im->data;
    im->data = expo;
    free(old);
    
    
}

void blend(Image *im1, Image *im2, double a)
{
    
  int cols = im1->cols;
  int rows = im1->rows;
  if (im2->cols > cols){
    cols = im2->cols;
  }
  if (im2->rows > rows){
    rows = im2->rows;
  }
  
  Pixel *data = malloc(cols*rows*3);
  for (int i = 0; i < cols*rows; i++)
  {
    Pixel *temp = data + i;
    temp->r = 0;
    temp->g = 0;
    temp->b = 0;
  }

  int im1count = 0;
  int im2count = 0;
  int count = 0;

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      int isIn1 = 0;
      int isIn2 = 0;
      if (i < im1->rows && j < im1->cols)
	{ isIn1 = 1; }
      if (i < im2->rows && j < im2->cols)
	{ isIn2 = 1; }

      if (isIn1 && isIn2){
	Pixel *im1p = im1->data + im1count;
        Pixel *im2p = im2->data + im2count;
	Pixel *temp = data + count;
	temp->r = (a * im1p->r) + ((1-a) * im2p->r);
	temp->g = (a * im1p->g) + ((1-a) * im2p->g);
	temp->b = (a * im1p->b) + ((1-a) * im2p->b);
	im1count++;
	im2count++;
      }
      else if (isIn1 && !isIn2){
	Pixel *im1p = im1->data + im1count;
	Pixel *temp = data + count;
	temp->r = im1p->r;
	temp->g = im1p->g;
	temp->b = im1p->b;
	im1count++;
      }
      else if (!isIn1 && isIn2){
	Pixel *im2p = im2->data + im2count;
	Pixel *temp = data + count;
	temp->r = im2p->r;
	temp->g = im2p->g;
	temp->b = im2p->b;
	im2count++;
      }
      count++;
    }
  }
  
  im1->cols = cols;
  im1->rows = rows;
  Pixel *old = im1->data;
  im1->data = data;
  free(old);
}

void zoom_in(Image *im)
{
    
    assert(im);
    int rows = im->rows;
    int cols = im->cols;

    Pixel* zoomin = malloc(cols * rows * 4 * 3);

    for (int i = 0; i < cols * rows * 4; i++)
    {
        Pixel* temp = zoomin + i;
        temp->r = 0;
        temp->g = 0;
        temp->b = 0;
    }
    
    int imcount = 0;


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Pixel* read = im->data + imcount;
            Pixel* temp = zoomin + i * 2 * cols*2 + j * 2;
            temp->r = read->r;
            temp->g = read->g;
            temp->b = read->b;
            temp = zoomin + i * 2 * cols*2 + j * 2 + 1;
            temp->r = read->r;
            temp->g = read->g;
            temp->b = read->b;
            temp = zoomin + (i * 2 + 1) * cols*2 + j * 2;
            temp->r = read->r;
            temp->g = read->g;
            temp->b = read->b;
            temp = zoomin + (i * 2 + 1) * cols*2 + j * 2 + 1;
            temp->r = read->r;
            temp->g = read->g;
            temp->b = read->b;
            imcount++;
        }
    }
    im->rows = rows*2;
    im->cols = cols*2;
    Pixel* old = im->data;
    im->data = zoomin;
    free(old);
}

void zoom_out(Image *im)
{
  
  int rows = im->rows/2;
  int cols = im->cols/2;
  Pixel* zoomout = malloc(rows*cols*3);
  for(int i=0; i<rows*cols;i++){
    Pixel* temp = zoomout + i;
        temp->r = 0;
        temp->g = 0;
        temp->b = 0;

  }
  
  int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
	  Pixel* temp = zoomout + count;
	  Pixel* read = im->data + (i*2)*(cols*2)+j*2;
	  temp->r= read->r;
      temp->g = read->g;
      temp->b = read->b;
	  read = im->data + (i*2)*(cols*2)+j*2+1;
      temp->r = temp->r/2 + read->r/2;
      temp->g = temp->g / 2 + read->g / 2;
      temp->b = temp->b / 2 + read->b / 2;
	  read = im->data + (i*2+1)*(cols*2)+j*2;
      temp->r = temp->r / 3 * 2 + read->r / 3;
      temp->g = temp->g / 3 * 2 + read->g / 3;
      temp->b = temp->b / 3 * 2 + read->b / 3;
	  read = im->data + (i*2+1)*(cols*2)+j*2+1;
      temp->r = temp->r / 4 * 3 + read->r / 4;
      temp->g = temp->g / 4 * 3 + read->g / 4;
      temp->b = temp->b / 4 * 3 + read->b / 4;
	  count++;
        }
    }
    im->rows = rows;
    im->cols = cols;
    Pixel* old = im->data;
    im->data = zoomout;
    free(old);


}

void copy_color(Pixel *ref, Pixel *copy)
{
  copy->r = ref->r;
  copy->g = ref->g;
  copy->b = ref->b;
}

void pointilism(Image *im)
{
    
  int num_pixels = im->cols * im->rows;
  int lim = (int)(num_pixels * 0.03);

  for (int i = 0; i < lim; i++)
  {
    srand(i);
    int pos = rand() % num_pixels;
    if (pos < im->cols*4 || pos > (num_pixels-im->cols*4))
      {pos = num_pixels/2;}
    
    int radius = (rand() % 5) + 1;
    Pixel *p = im->data + pos;
    
    copy_color(p, p-1);
    copy_color(p, p+1);
    copy_color(p, p - im->cols);
    copy_color(p, p + im->cols);
    
    if (radius >= 2)
    {
      copy_color(p, p-2);
      copy_color(p, p+2);
      copy_color(p, p - (im->cols*2));
      copy_color(p, p + (im->cols*2));
      copy_color(p, p+1+(im->cols));
      copy_color(p, p-1+(im->cols));
      copy_color(p, p-1-(im->cols));
      copy_color(p, p+1-(im->cols));
    }
    if (radius >= 3)
    {
      copy_color(p, p-3);
      copy_color(p, p+3);
      copy_color(p, p - (im->cols*3));
      copy_color(p, p + (im->cols*3));
      copy_color(p, p-2-(im->cols*2));
      copy_color(p, p+2-(im->cols*2));
      copy_color(p, p+2+(im->cols*2));
      copy_color(p, p-2+(im->cols*2));
      copy_color(p, p-1-(im->cols*2));
      copy_color(p, p+1-(im->cols*2));
      copy_color(p, p+1+(im->cols*2));
      copy_color(p, p-1+(im->cols*2));
      copy_color(p, p-2-(im->cols));
      copy_color(p, p+2-(im->cols));
      copy_color(p, p+2+(im->cols));
      copy_color(p, p-2+(im->cols));
    }
    if (radius >= 4)
    {
      copy_color(p, p-4);
      copy_color(p, p+4);
      copy_color(p, p - (im->cols*4));
      copy_color(p, p + (im->cols*4));
      copy_color(p, p-2-(im->cols*3));
      copy_color(p, p+2-(im->cols*3));
      copy_color(p, p+2+(im->cols*3));
      copy_color(p, p-2+(im->cols*3));
      copy_color(p, p-1-(im->cols*3));
      copy_color(p, p+1-(im->cols*3));
      copy_color(p, p+1+(im->cols*3));
      copy_color(p, p-1+(im->cols*3));
      copy_color(p, p-3-(im->cols*2));
      copy_color(p, p+3-(im->cols*2));
      copy_color(p, p+3+(im->cols*2));
      copy_color(p, p-3+(im->cols*2));
      copy_color(p, p-3-im->cols);
      copy_color(p, p+3-im->cols);
      copy_color(p, p+3+im->cols);
      copy_color(p, p-3+im->cols);
    }
    if (radius >= 5)
    {
      copy_color(p, p-3-(im->cols*3));
      copy_color(p, p+3-(im->cols*3));
      copy_color(p, p+3+(im->cols*3));
      copy_color(p, p-3+(im->cols*3));
      copy_color(p, p-4-im->cols);
      copy_color(p, p+4-im->cols);
      copy_color(p, p+4+im->cols);
      copy_color(p, p-4+im->cols);
      copy_color(p, p-1-(im->cols*4));
      copy_color(p, p+1-(im->cols*4));
      copy_color(p, p+1+(im->cols*4));
      copy_color(p, p-1+(im->cols*4));
    }
  }
}

void swirl(Image *im, int x, int y, int s)
{
    
  Pixel *swirl = malloc(im->cols*im->rows*3);

  for (int i = 0; i < im->cols*im->rows; i++)
  {
    Pixel *temp = swirl + i;
    temp->r = 0;
    temp->g = 0;
    temp->b = 0;
  }
  
  int count = 0;
  
  for (int j = 0; j < im->rows; j++)
  {
    for (int i = 0; i < im->cols; i++)
    {
      double alpha = (sqrt((i-x)*(i-x) + (j-y)*(j-y))) / s;
      int newX = (i-x)*cos(alpha) - (j-y)*sin(alpha) + x;
      int newY = (i-x)*sin(alpha) + (j-y)*cos(alpha) + y;
      
      if (newX >= 0 && newX < im->cols && newY >= 0 && newY < im->rows){
	int pos = newY * im->cols + newX;
	Pixel *p = im->data + pos;
	Pixel *newP = swirl + count;
	copy_color(p, newP);
      }
      count++;
    }
  }
  
  Pixel *old = im->data;
  im->data = swirl;
  free(old);
}


double** Gaussian(double sigma, int N) {
    double** gaussian = malloc(sizeof(double*) * N);
    for (int i = 0; i < N; i++) {
        gaussian[i] = malloc(sizeof(double) * N);
    }
    if (!gaussian) {
        return NULL;
    }

    for (int i = 0; i <= N/2; i++) {
        for (int j = 0; j <= N/2; j++) {
            double g = (1.0 / (2.0 * M_PI * sigma * sigma)) * exp(-(i * i + j * j)) / (2 * sigma * sigma);
            if (N / 2 + 1 + i <= N && N / 2 + 1 + j <= N) {
                gaussian[N / 2 + j][N / 2 + i] =2* g;
                gaussian[N / 2 + j][N / 2 - i] =2* g;
                gaussian[N / 2 - j][N / 2 + i] =2* g;
                gaussian[N / 2 - j][N / 2 - i] =2* g;
            }
        }
    }
    return gaussian;
}

void apply(Image* im, Image* blur, int r, int c, int N, double** gaussian) {
    double normal = 0;
    double red = 0;
    double green = 0;
    double blue = 0;
    for (int x = -N/2; x <= N / 2; x++) {
        for (int y = -N/2; y <= N / 2; y++) {
            if (c + x >= 0 && c + x < im->cols && r + y >= 0 && r + y < im->rows) {
                red += im->data[(r + y) * im->cols + (c + x)].r * gaussian[y + N / 2][x + N / 2];
                green += im->data[(r + y) * im->cols + (c +x)].g * gaussian[y + N / 2][x + N / 2];
                blue += im->data[(r + y) * im->cols + (c + x)].b * gaussian[y + N / 2][x + N / 2];
                normal += gaussian[y + N / 2][x + N / 2];
            }
        }
    }
    red /= normal;
    green /= normal;
    blue /= normal;

    if (red > 255.0) {
        blur->data[r * blur->cols + c].r = 255;
    }
    else {
        blur->data[r * blur->cols + c].r = (unsigned char)red;
    }
    if (green > 255.0) {
        blur->data[r * blur->cols + c].g = 255;
    }
    else {
        blur->data[r * blur->cols + c].g = (unsigned char)green;
    }
    if (blue > 255.0) {
        blur->data[r * blur->cols + c].b = 255;
    }
    else {
        blur->data[r * blur->cols + c].b = (unsigned char)blue;
    }
}

Image* make(int rows, int cols) {
    Image* im = malloc(sizeof(Image));
    Pixel* pi = malloc(sizeof(Pixel) * rows * cols);
    im->cols = cols;
    im->rows = rows;
    im->data = pi;
    return im;
}

void blur(Image* im, double sigma)
{
    Image* blur = make(im->rows, im->cols);
    
   
    int N = (int)(ceil(10 * sigma)) % 2 == 1 ?ceil(10 * sigma) :ceil(10 * sigma) + 1;
    double** gaussian = Gaussian(sigma, N);
    
    for (int i = 0; i < im->rows;i++) {
        for (int j = 0; j < im->cols;j++) {
            apply(im, blur, i, j, N, gaussian);
        }
    }

    for (int i = 0; i < N;i++) {
        free(gaussian[i]);
    }
    free(gaussian);
    
    int count = 0;
    for (int i = 0; i <  (im->rows*im->cols); i++) {
	  Pixel* temp = im->data+count;
	  Pixel* read = blur->data + count;
	  temp->r=read->r;
	  temp->g=read->g;
	  temp->b=read->b;
	  count++;
    }
    free(blur);

}
