//
//  arquivo.c
//  Imagem04
//
//  Created by Marcelo Cendron on 28/04/16.
//  Copyright © 2016 Marcelo Cendron. All rights reserved.
//

#include "main.h"

PPMImage *readPPM(const char *filename)
{
    char buff[16];
    PPMImage *img;
    FILE *fp;
    int c, rgb_comp_color;
    //Abre arquivo PPM para leitura
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Não foi possível abrir o arquivo '%s'\n", filename);
        exit(1);
    }
    
    //Lê a primeira linha do arquivo
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }
    
    //O PPM inicia com "P6", nesse if verifica essa condição
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Formato de imagem inválido\n");
        exit(1);
    }
    
    //Aloca a estrutura
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) {
        fprintf(stderr, "Nao foi possivel alocar memoria\n");
        exit(1);
    }
    
    //Verificar se tem comentários na imagem:
    c = getc(fp);
    while (c == '#') {
        while ((c = getc(fp)) != '\n');
        c = getc(fp);
    }
    ungetc(c, fp);
    
    // Lê as dimensões da imagem:
    if (fscanf(fp, "%d %d", &img->width, &img->height) != 2) {
        fprintf(stderr, "Tamanho da imagem inválido (Erro carregando '%s')\n", filename);
        exit(1);
    }
    fprintf(stderr, "Tamanho da imagem: %d x %d\n", img->width, img->height);
    
    //Lê o padrão de cores (geralmente 255)
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Inválido componentes RGB (Erro carregando '%s')\n", filename);
        exit(1);
    }
    
    //Se for 8 bit (rgb_comp_color == 255)
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
        fprintf(stderr, "'%s' não tem componentes de 8-bits\n", filename);
        exit(1);
    }
    
    while (fgetc(fp) != '\n') ;
    //Aloca um pixel por vez
    img->data = (PPMPixel*)malloc(img->width * img->height * sizeof(PPMPixel));
    
    if (!img->data) {
        fprintf(stderr, "Não foi possível alocar a memória\n");
        exit(1);
    }
    
    //Lê pixel
    if (fread(img->data, 3 * img->width, img->height, fp) != img->height) {
        fprintf(stderr, "Erro carregando imagem '%s'\n", filename);
        exit(1);
    }
    fprintf(stderr, "Arquivo carregado na memória\n");
    
    fclose(fp);
    return img;
}


void writePPM(const char *filename, PPMImage *img, byte TIPO_IMAGEM )
{
    
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }
    
    
    //write the header file
    //image format
    if (TIPO_IMAGEM == COLORIDO)
        fprintf(fp, "P6\n");
    else
        fprintf(fp, "P5\n");
    
    //comments
    fprintf(fp, "# Created by %s\n",CREATOR);
    
    //image size
    fprintf(fp, "%d %d\n",img->width,img->height);
    
    // Utilizado para indicar o maior tamanho da tonalidade
    if (TIPO_IMAGEM == COLORIDO)
        fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);
    else
        fprintf(fp, "%d\n",GRAY_COLOR);
    
    
    if (TIPO_IMAGEM == COLORIDO){ // Salvando a imagem em cores
        // pixel data
        fwrite(img->data, 3 * img->width, img->height, fp);
    }else{ // Grava a parte cinza da imagem
        fwrite(img->dataBW, img->width, img->height, fp);
        
    }
    
    fclose(fp);
}

