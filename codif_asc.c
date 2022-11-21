/******************************************************************
 * Arquivo: codif_asc.c                                            *
 *                                                                 *
 * Codifica um arquivo ascii segundo o dicionário QV fornecido, se-*
 * guindo a regra da distorção mínima (distância euclidiana).      *
 *                                                                 *
 * Autor: Francisco Madeiro Bernardino Junior                      *
 * UFPB - CCT - DEE - LABCOM                                       *
 *                                                                 * 
 * Modificado por: Waslon Terllizzie Araujo Lopes                  *
 * Data: 17/04/2001                                                *
 ******************************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include "alocacao.h"
#include "inout.h"
#define cnome 50 

int codifica(double **pta, double *y, int N, int K); 
void codifica_arquivo(char entrada[], char saida[], double **x, int N, int K);
double distancia(double *pta, double *ptb, int K);

int main()
{
    char dicion[cnome], codi[cnome], ent[cnome];
    int   K, N;
    double **pta;

    printf("Qual a dimensão do quantizador?\n");
    scanf("%d", &K);

    printf("Qual o número de níveis do dicionário?\n");
    scanf("%d", &N);

    printf("Qual o nome do arquivo que contém o dicionário ?\n");
    scanf("%s", dicion);

    printf("Qual o nome do arquivo que contém os dados a serem codificados ?\n");
    scanf("%s", ent);

    printf("Qual o nome do arquivo de símbolos codificados?\n");
    scanf("%s", codi);

    pta = ler_arquivo(dicion,N,K);

    codifica_arquivo(ent,codi,pta,N,K); 

    return (0);

} /* fim do main */

int codifica(double **pta, double *y, int N, int K){ 
    //Retorna o índice do vetor-código  que produz a menor distorção 
    //Dicionário (pta) - Vetor (y) - Número de vetores do dicionário (N)

    int i,cod;
    double dist;

    dist = distancia(pta[0],y,K);
    cod = 0;

    for(i=1;i<N;i++)
        if(dist > distancia(pta[i],y,K) ){
            dist = distancia(pta[i],y,K);
            cod = i;
        }

    return cod;
}

double distancia(double *pta, double *ptb, int K)
    // Calcula a distancia euclidiana entre os vetores (dimensao K) pta e ptb
{
    int i;
    double dist;

    dist = 0.0;

    for(i=0;i<K;i++)
        dist += pow(pta[i] - ptb[i],2.0);

    dist = sqrt(dist);

    return dist;
}


void codifica_arquivo(char entrada[], char saida[], double **x, int N, int K){
    /******************************************************************
     * Função que codifica os vetores do arquivo de nome "entrada" em  *
     * índices armazenados no arquivo de nomes "saída", de acordo com  *
     * o dicionário "x" de dimensão N,K.                               *
     ******************************************************************/

    FILE *entradap, *saidap;
    int i, j;
    double *y;

    y = aloca_vetor_double(K);  

    entradap = fopen(entrada,"r");
    if (!entradap) {
        printf("\nErro na abertura do arquivo dos dados a serem codificados\n");
        exit(1);
    }

    saidap = fopen(saida,"w");
    if (!saidap) {
        printf("\nErro na abertura do arquivo dos dados codificados\n");
        exit(1);
    }


    for(i=0;!feof(entradap);i+=K){
        fscanf(entradap,"%lf",&y[0]);
        if( feof(entradap) ) break;
        for(j=1;j<K;j++)
            fscanf(entradap,"%lf",&y[j]);
        fprintf(saidap,"%d\n",codifica(x,y,N,K));
        for(j=0;j<K;j++)
            y[j] = 0.0;
    }

    fclose(saidap);
    fclose(entradap);

    return;
}

