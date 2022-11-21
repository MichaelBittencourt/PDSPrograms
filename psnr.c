/******************************************************************
 *  Programa psnr.c                                                *
 *                                                                 *
 *  Calcula a métrica PSNR (Peak Signal-to-Noise Ration entre duas *
 *  imagens no formato PGM.                                        *
 *                                                                 *
 *  Autor: Waslon Terllizzie Araújo Lopes                          *
 *  Data: 12/11/2016                                               *
 *                                                                 *
 *  Uso: psnr.out imagemA.pgm imagemB.pgm                          *
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "inout.h"
#define cnome 50 /* comprimento maximo de nome de arquivo */


int main(int argc, char *argv[])
{

    /* definicao de variaveis */

    char orig[cnome], quant[cnome];
    int i, j, m, n;
    double  psnr, varxyseg, **ptx, **pty;

    /* Faz a leitura das duas imagens */
    if(argc!=3){
        printf("Qual o nome do arquivo original?\n");
        scanf("%s", orig); 
        printf("Qual o nome do arquivo de dados quantizados?\n");
        scanf("%s", quant);

        ptx = ler_arquivo_pgm(orig,&n,&m);
        pty = ler_arquivo_pgm(quant,&n,&m);
    } 
    else {
        ptx = ler_arquivo_pgm(argv[1],&n,&m);
        pty = ler_arquivo_pgm(argv[2],&n,&m);
    }

    varxyseg=0.0;
    for(i=0;i<n;i++)
        for(j=0;j<m;j++)
            varxyseg += pow((ptx[i][j]-pty[i][j]),2.0);
    varxyseg /= (n*m);

    printf("O erro médio quadrático é MSE = %lf\n",varxyseg);
    psnr=10*(log10(255.0*255.0)-log10(varxyseg)); 
    printf("A relação Sinal/Ruído de Pico é PSNR = %lf\n",psnr);

    return (0);
} 
