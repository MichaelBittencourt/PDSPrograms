/******************************************************************
 *	Programa dct_voz.c                                             *
 *                                                                 *
 *  Faz a DCT direta e inversa de um sinal de voz com descarte de  *
 *  coeficientes da DCT.                                           *
 *                                                                 *
 *  Z. T. Drweesh and L. E. George, "Audio Compression Based on    *
 *  Discrete Cosine Transform, Run Length and High Order Shift     *
 *  Encodig", International Journal of Engineering and Innovative  *
 *  Technology, pp. 45-51, Vol. 4, Issue 1, July 2014.             *
 *                                                                 *
 *  Autor: Waslon Terllizzie Araújo Lopes                          *
 *  Data: 20/11/2020                                               *
 *                                                                 *
 *  Uso: dct_voz.out entrada.dat saida.dat tamanho_dct descarte    *
 ******************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "inout.h"
#define cnome 100 /* comprimento max de nome de arquivo */

#define Pi acos(-1)

void dct(double *x, double *y, int n_amos, int dct_length, int dct_descarte);
void dct_inversa(double *x, double *y, int n_amos, int dct_length);
void funcao(double *C, double *f, int N, int dct_descarte);
void funcao_inversa(double *C, double *f, int N);

int main (int argc, char **argv){

    FILE *f;
    char entrada[cnome],quant[cnome],*str,*str2;
    int n_amos,dct_length,dct_descarte,i;
    double *x,*y;

    /* Abre os arquivos de entrada e saída */
    if(argc!=5){
        printf("Qual o nome do arquivo original (*.dat)?\n");
        scanf("%s", entrada); 
        x = ler_vetor_arquivo_treino(entrada,&n_amos);

        printf("Qual o nome do reconstruído após descarte DCT (*.dat)?\n");
        scanf("%s", quant);
        f = fopen(quant, "w");

        printf("Qual o comprimento da DCT?\n");
        scanf("%d",&dct_length);

        printf("Quantos coeficientes devem ser descartados?\n");
        scanf("%d",&dct_descarte);
    } 
    else{
        x = ler_vetor_arquivo_treino(argv[1],&n_amos);
        f = fopen(argv[2], "w");
        str = argv[3];
        dct_length = atoi(str);
        str2 = argv[4];
        dct_descarte = atoi(str2);
    }

    y = aloca_vetor_double(n_amos);

    // Cálculo da DCT
    dct(x, y, n_amos, dct_length, dct_descarte);

    // Cálculo da DCT inversa
    dct_inversa(y, x, n_amos, dct_length);

    for(i=0;i<n_amos;i++)
        fprintf(f,"%.10f\n",x[i]);

    free(x);
    free(y);
    fclose(f);
    return (1);
}


void dct(double *x, double *y, int n_amos, int dct_length, int dct_descarte){

    int n_blocos, i;

    n_blocos = n_amos/dct_length;

    for(i=0;i<n_blocos;i++){
        funcao(x,y,dct_length,dct_descarte);
        x += dct_length;
        y += dct_length;
    }

    x -= (n_blocos*dct_length);
    y -= (n_blocos*dct_length);

    return;
}

void dct_inversa(double *y, double *x, int n_amos, int dct_length){

    int n_blocos, i;

    n_blocos = n_amos/dct_length;

    for(i=0;i<n_blocos;i++){
        funcao_inversa(y,x,dct_length);
        x += dct_length;
        y += dct_length;
    }

    x -= (n_blocos*dct_length);
    y -= (n_blocos*dct_length);

    return;
}

void funcao(double *f, double *C, int N, int dct_descarte){
    int u,t;
    // Implementa a Eq. 1 do paper -> DCT direta
    for(u=0;u<N;u++){
        C[u] = 0;
        for(t=0;t<N;t++)
            C[u] += cos(Pi*u/(2*N)* (2*t+1))*f[t]; 
        if(!u)
            C[u] *= sqrt(1.0/N);
        else
            C[u] *= sqrt(2.0/N);
    }
    // O descarte de coeficientes é feito aqui
    for(u=N-dct_descarte;u<N;u++)
        C[u] = 0.0;

    return;
}

void funcao_inversa(double *C, double *f, int N){
    // Implementa a Eq. 3 do paper -> DCT inversa

    int u,t;

    for(t=0;t<N;t++){
        f[t] = 0;
        for(u=0;u<N;u++)
            if(!u)
                f[t] += sqrt(1.0/N) * cos(Pi*u/(2*N)* (2*t+1))*C[u]; 
            else
                f[t] += sqrt(2.0/N) * cos(Pi*u/(2*N)* (2*t+1))*C[u]; 
    }
    return;
}
