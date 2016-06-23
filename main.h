//
//  main.h
//  Imagem04
//
//  Created by Marcelo Cendron on 30/04/16.
//  Copyright © 2016 Marcelo Cendron. All rights reserved.
//

#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define PROCNUM 4 // Número de processadores
#define COLORIDO 1
#define CINZA 0
#define PI 3.1415926535
#define SQR(x) ((x)*(x))

#define CREATOR "SEU NOME"
#define RGB_COMPONENT_COLOR 255
#define GRAY_COLOR 255

typedef unsigned char byte;

typedef struct {
    byte red,green,blue;
} PPMPixel;

typedef struct {
    int width, height;
    PPMPixel *data;
    byte *dataBW;
} PPMImage;


// arquivo.c:
PPMImage *readPPM(const char *filename);
void writePPM(const char *filename, PPMImage *img, byte TIPO_IMAGEM );

// processos.c:
int MIN(int X, int Y);
int MAX(int X, int Y);
PPMImage *converteBW(PPMImage *img);
void *histograma (PPMImage *histImage);
PPMImage *negativo (PPMImage *img);
PPMImage *aplicaSepia(PPMImage *img);
PPMImage *aumentaBrilho(PPMImage *img, unsigned char valor);
PPMImage *aplicaThresold(PPMImage *img, unsigned char valor);

//convolucao.c:
PPMImage *convolucao (PPMImage *im, int mask[3][3], int MASK_COLUNAS, int MASK_LINHAS);
PPMImage *convolucaoBW (PPMImage *im, int mask[3][3], int MASK_COLUNAS, int MASK_LINHAS);

// hough.c:
float **createMatrix(int height, int width);
PPMImage *processImageHough (float ** x, int nr, int nc);
PPMImage *hough(PPMImage *image);
float **houghTransformLines(PPMImage *imgHough);


#endif /* main_h */
