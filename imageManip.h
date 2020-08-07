//emill105
//Ely Miller
//yli346
//yuntao Li

#ifndef IMAGEMANIP_H
	#define M_PI 3.14159265358979323846
#define IMAGEMANIP_H

#include <stdio.h>
#include <math.h>
#include "ppm_io.h"

void exposure(Image *, double);

void blend(Image *, Image *, double);

void zoom_in(Image *);

void zoom_out(Image *);

void pointilism(Image *);

void swirl(Image *, int, int, int);

void blur(Image *, double);

#endif
