/******************************************************************
 *	Programa raw2dat.c                                             *
 *                                                                 *
 *  Transforma um arquivo de áudio binário  8 bits em um arquivo   *
 *  de texto (-1 <= x < +1).                                       *
 *                                                                 *
 *  Autor: Waslon Terllizzie Araújo Lopes                          *
 *  Data: 29/01/2017                                               *
 *                                                                 *
 *  Uso: raw2dat.out entrada.raw saida.dat                         *
 ******************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "inout.h"
#define cnome 100 /* comprimento max de nome de arquivo */

int main (int argc, char **argv){

    FILE *f,*saida;
    char amos[cnome],quant[cnome];
    int8_t bytes[1];
    unsigned int t, n_amost;
    double y;


    /* Abre os arquivos de entrada e saída */
    if(argc!=3){
        printf("Qual o nome do arquivo original (*.raw)?\n");
        scanf("%s", amos); 
        f = fopen(amos, "rb");

        printf("Qual o nome do arquivo de aúdio (*.dat)?\n");
        scanf("%s", quant);
        saida = fopen(quant, "wb");
    } 
    else {
        f = fopen(argv[1], "rb");
        saida = fopen(argv[2], "wb");
    }

    for (t = 0; !feof(f); t++) {
        fread(bytes, 1, sizeof(int8_t), f);
    }

    n_amost = t-1;

    rewind(f);

    for (t = 0; t<n_amost ; t++) {
        fread(bytes, 1, sizeof(int8_t), f);
        y = (1.0*bytes[0])/INT8_MAX;
        fprintf(saida,"%.10f \n",y); 
    }

    fclose(f);
    fclose(saida);
    return(1);
}
