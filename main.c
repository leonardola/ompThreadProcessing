//
//  main.c
//  Imagem04
//
//  Created by Marcelo Cendron on 28/04/16.
//  Copyright © 2016 Marcelo Cendron. All rights reserved.
//

#include "main.h"

int main(int argc, const char * argv[]) {
    clock_t  start, time_taken_millis;
    PPMImage *image, *image2, *imageBW;
    double startime,endtime;
    
    
    image = readPPM("img02.ppm");
    
    startime = omp_get_wtime();
    imageBW = converteBW(image);
    endtime = omp_get_wtime();
    printf("Tempo total %lf microsegundos\n\n",endtime-startime);
    writePPM("imagem_BW.ppm",imageBW, CINZA);
    
    startime = omp_get_wtime();
    image2 = negativo(image );
    endtime = omp_get_wtime();
    printf("Tempo total %lf microsegundos\n\n",endtime-startime);
    writePPM("imagem_negativo.ppm",image2,  COLORIDO);
    free(image2);
    
    startime = omp_get_wtime();
    image2 = aplicaSepia(image);
    endtime = omp_get_wtime();
    printf("Tempo total %lf microsegundos\n\n",endtime-startime);
    writePPM("imagem_sepia.ppm",image2, COLORIDO);
    free(image2);
    
    start = clock();
    image2 = aumentaBrilho(image, 100);
    time_taken_millis = (clock()-start);
    printf("Tempo total %lu microsegundos\n\n",time_taken_millis);
    writePPM("imagem_brilho.ppm",image2,  COLORIDO);
    free(image2);
    
    
    start = clock();
    image2 = aplicaThresold(imageBW, 127);
    time_taken_millis = (clock()-start);
    printf("Tempo total %lu microsegundos\n\n",time_taken_millis);
    writePPM("imagem_Threasold.ppm",image2, CINZA);
    free(image2);
    
    int filter[3][3] = {
        {0, 0, 0},
        {-1, 1, 0},
        {0, 0, 0}
    };
    
    start = clock();
    image2 = convolucaoBW(imageBW, filter, 3, 3);
    time_taken_millis = (clock()-start);
    printf("Tempo total %lu microsegundos\n\n",time_taken_millis);
    writePPM("imagem_ConvolucaoBW.ppm",image2, CINZA);
    free(image2);
    
    start = clock();
    image2 = convolucao(image, filter, 3, 3);
    time_taken_millis = (clock()-start);
    printf("Tempo total %lu microsegundos\n\n",time_taken_millis);
    writePPM("imagem_Convolucao.ppm",image2, COLORIDO);
    free(image2);
    
    
    start = clock();
    image2 = hough(imageBW);
    time_taken_millis = (clock()-start);
    printf("Tempo total %lu microsegundos\n\n",time_taken_millis);
    writePPM("imagem_Hough.ppm",image2, CINZA);
    free(image2);
    
    free(imageBW);
}
