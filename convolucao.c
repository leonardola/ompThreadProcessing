//
//  convolucao.c
//  Imagem04
//
//  Created by Marcelo Cendron on 30/04/16.
//  Copyright © 2016 Marcelo Cendron. All rights reserved.
//

#include "main.h"

PPMImage *convolucao (PPMImage *img, int mask[3][3], int MASK_COLUNAS, int MASK_LINHAS)
{
    int i, j, ii, jj, n, m ;
    
    float  factor = 1.0;
    float bias = 0.0;
    
    PPMImage *imgConvul;
    imgConvul = (PPMImage *)malloc(sizeof(PPMImage));
    
    //Aloca o espaço para os pixeis
    imgConvul->data = (PPMPixel*)malloc(img->width * img->height * sizeof(PPMPixel));
    
    if (!imgConvul | !imgConvul->data) {
        fprintf(stderr, "Nao foi possivel alocar memoria\n");
        exit(1);
    }
    
    fprintf(stderr, "Aplicando a convolução...");
    
    imgConvul->width = img->width;
    imgConvul->height = img->height;
    
    //apply the filter
    for(i = 0; i < img->width; i++){
        for(j = 0; j < img->height; j++)
        {
            float red = 0, green = 0, blue = 0;
            
            //multiply every value of the filter with corresponding image pixel
            for(ii = 0; ii < MASK_LINHAS; ii++){
                n = i - (MASK_LINHAS / 2) + ii;
                if (n<0 || n>=img->width)
                    continue;
                for(jj = 0; jj < MASK_COLUNAS; jj++)
                {
                    m = j - (MASK_COLUNAS / 2) + jj;
                    if (m<0 || m>=img->height)
                        continue;
                    // fprintf(stderr, "max: %d, atual: %d x %d = %d\n", img->width*img->height, imageX, imageY, imageX * img->height + imageY);
                    red += img->data[n * img->height + m].red * mask[ii][jj];
                    green += img->data[n * img->height + m].green * mask[ii][jj];
                    blue += img->data[n * img->height + m].blue * mask[ii][jj];
                }
                
                
                //truncate values smaller than zero and larger than 255
                imgConvul->data[i * img->height + j].red = MIN(MAX((factor * red + bias), 0), 255);
                imgConvul->data[i * img->height + j].green = MIN(MAX((factor * green + bias), 0), 255);
                imgConvul->data[i * img->height + j].blue = MIN(MAX((factor * blue + bias), 0), 255);
            }
        }
    }
    
    fprintf(stderr, "pronto\n");
    
    
    return imgConvul;
}

PPMImage *convolucaoBW (PPMImage *img, int mask[3][3], int MASK_COLUNAS, int MASK_LINHAS)
{
    int i, j, ii, jj, n, m ;
    
    float  factor = 1.0;
    float bias = 0.0;
    
    if (!img->dataBW) {
        fprintf(stderr, "A imagem passada deve ser preta e branca\n");
        exit(1);
    }
    
    
    PPMImage *imgConvul;
    imgConvul = (PPMImage *)malloc(sizeof(PPMImage));
    
    //Aloca o espaço para os pixeis
    imgConvul->dataBW = (byte*)malloc(img->width * img->height * sizeof(byte));
    
    if (!imgConvul | !imgConvul->dataBW) {
        fprintf(stderr, "Nao foi possivel alocar memoria\n");
        exit(1);
    }
    
    fprintf(stderr, "Aplicando a convolução BW...");
    
    imgConvul->width = img->width;
    imgConvul->height = img->height;
    
    //apply the filter
    for(i = 0; i < img->width; i++){
        for(j = 0; j < img->height; j++)
        {
            float tmp = 0;
            
            //multiply every value of the filter with corresponding image pixel
            for(ii = 0; ii < MASK_LINHAS; ii++){
                n = i - (MASK_LINHAS / 2) + ii;
                if (n<0 || n>=img->width)
                    continue;
                for(jj = 0; jj < MASK_COLUNAS; jj++)
                {
                    m = j - (MASK_COLUNAS / 2) + jj;
                    if (m<0 || m>=img->height)
                        continue;
                    // fprintf(stderr, "max: %d, atual: %d x %d = %d\n", img->width*img->height, imageX, imageY, imageX * img->height + imageY);
                    tmp += img->dataBW[n * img->height + m] * mask[ii][jj];
                }
                
                
                //truncate values smaller than zero and larger than 255
                imgConvul->dataBW[i * img->height + j] = MIN(MAX((factor * tmp + bias), 0), 255);
            }
        }
    }
    
    fprintf(stderr, "pronto\n");
    
    
    return imgConvul;
}
