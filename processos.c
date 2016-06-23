//
//  processos.c
//  Imagem04
//
//  Created by Marcelo Cendron on 30/04/16.
//  Copyright © 2016 Marcelo Cendron. All rights reserved.
//

#include "main.h"

int MIN(int X, int Y){
    return (X < Y) ? X : Y;
}
int MAX(int X, int Y){
    return (X > Y) ? X : Y;
}

PPMImage *converteBW(PPMImage *img)
{
    int i;
    PPMImage *imgBW;
    int tmp;
    
    //Aloca a estrutura
    imgBW = (PPMImage *)malloc(sizeof(PPMImage));
    
    //Aloca o espaço para os pixeis
    imgBW->dataBW = (byte*)malloc(img->width * img->height * sizeof(byte));
    
    if (!imgBW | !imgBW->dataBW) {
        fprintf(stderr, "Nao foi possivel alocar memoria\n");
        exit(1);
    }
    
    imgBW->width = img->width;
    imgBW->height = img->height;
    
    // SEÇÃO PASSÍVEL DE OTIMIZAÇÃO
    fprintf(stderr, "Convertendo para escala de cinza...");
    
	//PRIMEIRA APLICAÇÃO OPENMP
    #pragma omp parallel for num_threads(4) private(i, tmp)  
    for ( i = 0; i < (img->width * img->height); i++ ) {        
        
        tmp = (byte) ( img->data[i].red * 0.299 +
                      img->data[i].green * 0.587 +
                      img->data[i].blue * 0.114);
        imgBW->dataBW[i] = tmp > 255? 255 : tmp;
        
    }
    fprintf(stderr, "pronto\n");
    return imgBW;
}

void *histograma (PPMImage *histImage)
{
    int i, *hist;
    
    //Aloca o espaço para os pixeis
    hist = (int *)malloc(256 * sizeof(int));
    
    if (!histImage->dataBW) {
        fprintf(stderr, "A imagem passada deve ser preta e branca\n");
        exit(1);
    }
    
    if (!hist) {
        fprintf(stderr, "Nao foi possivel alocar memoria\n");
        exit(1);
    }
    fprintf(stderr, "Gerando o histograma...");
    
    /* Zera todos os elementos do histograma. */
    for (i=0; i<256; i++)
        hist[i] = 0;
    
    for (i=0; i < (histImage->width * histImage->height); i++)
        hist[histImage->dataBW[i]]++;
    
    fprintf(stderr, "pronto\n");
    fprintf(stderr, "Imprimindo histograma...\n");
    
    for(i=0; i <= 255; i++)
    {
        printf("%3d%10d", i, hist[i]);
        printf("\n");
    }
    
    /*
     int *hist2 = (int *)malloc(256 * sizeof(int));
     for (i=0; i<256; i++)
     hist2[i] = 0;
     
     for (i=0; i < (histImage->width * histImage->height); i++)
     hist2[histImage->dataBW[i]]++;
     
     
     fprintf(stderr, "pronto\n");
     fprintf(stderr, "!!!!Comparando os histogramas...\n");
     
     for(i=0; i <= 255; i++)
     {
     if (hist[i] != hist2[i]){
     printf("Diferente: %3d%10d\n", i, hist[i]);
     }
     }
     
     fprintf(stderr, "pronto\n");
     */
    
    return hist;
    
}

PPMImage *negativo (PPMImage *img)
{
    int i;
    int r, g, b;
    PPMImage *imgNegativo;
    imgNegativo = (PPMImage *)malloc(sizeof(PPMImage));
    
    //Aloca o espaço para os pixeis
    imgNegativo->data = (PPMPixel*)malloc(img->width * img->height * sizeof(PPMPixel));
    
    if (!imgNegativo | !imgNegativo->data) {
        fprintf(stderr, "Nao foi possivel alocar memoria\n");
        exit(1);
    }
    
    imgNegativo->width = img->width;
    imgNegativo->height = img->height;
    
    
    fprintf(stderr, "Aplicando o negativo...");
    
    //SEGUNDA APLICAÇÃO OPENMP
	#pragma omp parallel for num_threads(4) private(i, r, g, b)  
    for ( i = 0; i < (img->width * img->height); i++ ) {
        r = 255 - img->data[i].red;
        imgNegativo->data[i].red = r < 0? 0 : r;
        
        g = 255 - img->data[i].green;
        imgNegativo->data[i].green = g < 0 ? 0 : g;
        
        b = 255 - img->data[i].blue;
        imgNegativo->data[i].blue = b < 0 ? 0 : b;
        
    }
    fprintf(stderr, "pronto\n");
    
    return imgNegativo;
    
}


PPMImage *aplicaSepia(PPMImage *img)
{
    int i;
    int r, g, b;
    PPMImage *imgsepia;
    //Aloca a estrutura
    imgsepia = (PPMImage *)malloc(sizeof(PPMImage));
    
    //Aloca o espaço para os pixeis
    imgsepia->data = (PPMPixel*)malloc(img->width * img->height * sizeof(PPMPixel));
    
    if (!imgsepia | !imgsepia->data) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    imgsepia->width = img->width;
    imgsepia->height = img->height;
    
    
    // SEÇÃO PASSÍVEL DE OTIMIZAÇÃO
    fprintf(stderr, "Aplicando o filtro sepia...");
    
	// TERCEIRA APLICAÇÃO OPENMP
	#pragma omp parallel for num_threads(4) private(i, r, g, b)    
    for ( i = 0; i < (img->width * img->height); i++ ) {
        r = 0.393 * img->data[i].red + 0.769 * img->data[i].green + 0.189 * img->data[i].blue;
        imgsepia->data[i].red = r > 255 ? 255 : r;
        
        g = 0.349 * img->data[i].red + 0.686 * img->data[i].green + 0.168 * img->data[i].blue;
        imgsepia->data[i].green = g > 255 ? 255 : g;
        
        b = 0.272 * img->data[i].red + 0.534 * img->data[i].green + 0.131 * img->data[i].blue;
        imgsepia->data[i].blue = b > 255 ? 255 : b;
        
    }
    fprintf(stderr, "pronto\n");
    return imgsepia;
}

PPMImage *aumentaBrilho(PPMImage *img, unsigned char valor)
{
    int i;
    PPMImage *imgBrilho;
    int r, g, b;
    //Aloca a estrutura
    imgBrilho = (PPMImage *)malloc(sizeof(PPMImage));
    
    //Aloca o espaço para os pixeis
    imgBrilho->data = (PPMPixel*)malloc(img->width * img->height * sizeof(PPMPixel));
    
    if (!imgBrilho | !imgBrilho->data) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    imgBrilho->width = img->width;
    imgBrilho->height = img->height;
    
    
    // SEÇÃO PASSÍVEL DE OTIMIZAÇÃO
    fprintf(stderr, "Aplicando brilho a mais...");
    for ( i = 0; i < (img->width * img->height); i++ ) {
        r = img->data[i].red + valor;
        imgBrilho->data[i].red = r > 255? 255 : r;
        
        g = img->data[i].green + valor;
        imgBrilho->data[i].green = g > 255? 255 : g;
        
        b =img->data[i].blue + valor;
        imgBrilho->data[i].blue = b > 255? 255 : b;
        
    }
    fprintf(stderr, "pronto\n");
    return imgBrilho;
}

PPMImage *aplicaThresold(PPMImage *img, unsigned char valor)
{
    int i;
    PPMImage *imgThresold;
    
    if (!img->dataBW){
        fprintf(stderr, "A imagem passada precisa ser convertida para escala de cinza\n");
        fprintf(stderr, "faça uma chamada a função converteBW(PPMImage *img) antes\n");
        return NULL;
        
    }
    //Aloca a estrutura
    imgThresold = (PPMImage *)malloc(sizeof(PPMImage));
    
    //Aloca o espaço para os pixeis
    imgThresold->dataBW = (byte*)malloc(img->width * img->height * sizeof(byte));
    
    if (!imgThresold | !imgThresold->dataBW) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    imgThresold->width = img->width;
    imgThresold->height = img->height;
    
    // SEÇÃO PASSÍVEL DE OTIMIZAÇÃO
    
    fprintf(stderr, "Aplicando o thresold...");
    for ( i = 0; i < (img->width * img->height); i++ ) {
        if(img->dataBW[i] < valor){
            imgThresold->dataBW[i] = 0;
        }else
            imgThresold->dataBW[i] = 255;
        
    }
    fprintf(stderr, "pronto\n");
    return imgThresold;
}




