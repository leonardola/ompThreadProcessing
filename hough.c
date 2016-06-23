//
//  hough.c
//  Imagem04
//
//  Created by Marcelo Cendron on 30/04/16.
//  Copyright © 2016 Marcelo Cendron. All rights reserved.
//

#include "main.h"

float **createMatrix(int height, int width)
{
    int i, j;
    float **mx;
    
    // SEÇÃO PASSÍVEL DE OTIMIZAÇÃO
    mx = (float **) malloc(sizeof(float *)* height);
    for (i = 0; i < height; i++)
    {
        mx[i] = (float *) malloc(sizeof(float)*width);
        for (j = 0; j < width; j++)
            mx[i][j] = 0.0;
    }
    return mx;
}

PPMImage *processImageHough (float ** x, int nr, int nc)
{
    float xmax, xmin, z, rng;
    int k=0;
    int i,j;
    
    PPMImage *houghTrans;
    houghTrans = (PPMImage *)malloc(sizeof(PPMImage));
    
    //Aloca o espaço para os pixeis
    houghTrans->dataBW = (byte*)malloc(nr * nc * sizeof(byte));
    
    if (!houghTrans | !houghTrans->dataBW) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    xmax = xmin = x[0][0];
    for (i=0; i<nr; i++)
        for (j=0; j<nc; j++)
        {
            if (xmax < x[i][j]) xmax = x[i][j];
            if (xmin > x[i][j]) xmin = x[i][j];
        }
    rng = xmax - xmin;
    
    for (i=0; i<nr; i++)
        for (j=0; j<nc; j++)
        {
            z = x[i][j];
            k = (unsigned char) ((z-xmin)/rng * 255);
            houghTrans->dataBW[i * nc + j] = k;
        }
    
    houghTrans->width = nc;
    houghTrans->height = nr;
    
    
    return houghTrans;
}

PPMImage *hough (PPMImage *image)
{
    float **z;
    int center_x, center_y, r, omega, i, j, rmax, tmax;
    float conv;
    float tmval;
    PPMImage *houghTrans;
    
    conv = 3.1415926535/180.0;
    center_x = image->height/2;
    center_y = image->width/2;
    rmax = (int)(sqrt((float)(image->height*image->height +
                              image->width*image->width)) /2.0);
    
    
    fprintf(stderr, "Aplicando a transformada de Hough...");
    
    z = createMatrix(180, 2*rmax+1);
    for (r = 0; r < 2 * rmax+1; r++)
        for (omega = 0; omega < 180; omega++)
            z[omega][r] = 0;
    
    tmax = 0; tmval = 0;
    for (i = 0; i < image->width; i++)
        for (j = 0; j < image->height; j++)
            if (image->dataBW[i * image->height + j])
                for (omega = 0; omega < 180; ++omega)
                {
                    r = (i - center_y) * sin((float)(omega*conv))
                    + (j - center_x) * cos((float)(omega*conv));
                    z[omega][rmax+r] += 1;
                }
    for (i=0; i<180; i++)
        for (j=0; j<2*rmax+1; j++)
            if (z[i][j] > tmval)
            {
                tmval = z[i][j];
                tmax = i;
            }
    fprintf(stderr, "pronto\n");
    
    houghTrans = processImageHough (z, 180, 2*rmax+1);
    
    free (z[0]);
    free (z);
    
    return houghTrans;
    
}

float **houghTransformLines(PPMImage *imgHough)
{
    int m, n, angle, dist;
    int mapHeight = imgHough->width/5;
    int mapWidth =imgHough->height/5;
    float alpha, maxD = sqrt((float) (SQR(mapHeight) + SQR(mapWidth)));
    float **mxParam = createMatrix(mapHeight, mapWidth);
    float **sincos = createMatrix(mapHeight, 2);
    
    
    // SEÇÃO PASSÍVEL DE OTIMIZAÇÃO
    for (angle = 0; angle < mapHeight; angle++)
    {
        alpha = -0.5*PI + 1.5*PI*(float) angle/(float) mapHeight;
        sincos[angle][0] = (float) mapWidth/maxD*sin(alpha);
        sincos[angle][1] = (float) mapWidth/maxD*cos(alpha);
    }
    fprintf(stderr, "Aplicando o a transformada de Hough...");
    
    // SEÇÃO PASSÍVEL DE OTIMIZAÇÃO
    // float inicio, fim;
    //    inicio = omp_get_wtime();
    for (m = 0; m < mapHeight; m++){
        for (n = 0; n < mapWidth; n++)
            for (angle = 0; angle < mapHeight; angle++)
            {
                dist = (int) ((float) m*sincos[angle][0] + (float) n*sincos[angle][1] + 0.5);
                if (dist >= 1){
                    mxParam[angle][dist] += imgHough->dataBW[m * mapHeight + n];
                }
            }
    }
    
    //   fim = omp_get_wtime();
    //   printf("\ntempo de processamento %.2f\n", fim - inicio);
    
    
    FILE *fp;
    //open file for output
    fp = fopen("hough.csv", "wb");
    if (!fp) {
        fprintf(stderr, "Hough problem'\n");
        exit(1);
    }
    
    processImageHough(mxParam, mapHeight, mapWidth);
    
    /*
     for (m = 0; m < mapHeight; m++){
     for (n = 0; n < mapWidth; n++){
     fprintf(fp, "%f;", mxParam[m][n]);
     }
     fprintf(fp, "\n");
     }
     */
    
    fprintf(stderr, "pronto\n");
    
    return mxParam;
}
