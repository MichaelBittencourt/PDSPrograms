/******************************************************************
 *	Programa snr.c                                                 *
 *                                                                 *
 *  Calcula a SNR e SNRsegmental entre dois arquivos de audio.     *
 *                                                                 *
 *  Autor: Waslon Terllizzie Araújo Lopes                          *
 *  Data: 31/01/2017                                               *
 *                                                                 *
 *  Uso: snr.out arquivo1.dat arquivo2.dat                         *
 ******************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "inout.h"
#define cnome 100 /* comprimento max de nome de arquivo */



int main (int argc, char **argv){

    char entrada[cnome],quant[cnome];
    int n_amos, n_amos2,i,j,nblocos;
    double *x,*y,snr,snrseg,sigmavarx,sigmavarxy;

    /* Abre os arquivos de entrada e saída */
    if(argc!=3){
        printf("Qual o nome do arquivo original (*.dat)?\n");
        scanf("%s", entrada); 
        x = ler_vetor_arquivo_treino(entrada,&n_amos);

        printf("Qual o nome do arquivo quantizado (*.dat)?\n");
        scanf("%s", quant);
        y = ler_vetor_arquivo_treino(quant,&n_amos2);

        if(n_amos != n_amos2){
            printf("\nArquivos com comprimentos distintos!!!\n");
            exit(1);
        }

    } 
    else {
        x = ler_vetor_arquivo_treino(argv[1],&n_amos);
        y = ler_vetor_arquivo_treino(argv[2],&n_amos2);

        if(n_amos != n_amos2){
            printf("\nArquivos com comprimentos distintos!!!\n");
            exit(1);
        }
    }

    // Cálculo da SNR (signal-to-noise ratio)
    sigmavarx = 0.0;
    sigmavarxy = 0.0;
    for(i=0;i<n_amos;i++){
        sigmavarx += x[i]*x[i];
        sigmavarxy += (x[i]-y[i])*(x[i]-y[i]);
    }
    snr = 10 * log10(sigmavarx/sigmavarxy);

    printf("SNR = %.2f dB\n",snr);

    // Cálculo da SNRseg (segmental signal-to-noise ratio)

    nblocos = (int) n_amos/160; // 20 ms x 8000 amostras/s = 160 amostras
    snrseg = 0.0;
    for(i=0;i<nblocos;i++){
        sigmavarx = 0.0;
        sigmavarxy = 0.0;
        for(j=0;j<160;j++){
            sigmavarx += x[i*160+j]*x[i*160+j];
            sigmavarxy += (x[i*160+j]-y[i*160+j])*(x[i*160+j]-y[i*160+j]);
        }
        snrseg += 10 * log10(sigmavarx/sigmavarxy);
    }
    snrseg /= nblocos;
    printf("SNRseg = %.2f dB\n",snrseg);

    free(x);
    free(y);
    return (1);
}


