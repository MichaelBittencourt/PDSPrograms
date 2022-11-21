/******************************************************************
 *	Programa lbg.c                                                 *
 *  Realiza o treinamento do dicionário pelo algoritmo LBG         *
 *  Modificado por: Waslon Terllizzie Araújo Lopes                 *
 *  Data: 17/04/2001                                               *
 ******************************************************************/

#include <stdio.h>
#include "alocacao.h"
#include "inout.h"
#include <stdlib.h>
#include <math.h>
#define cnome 100 /* comprimento maximo de nome de arquivo,incluindo diretorio */

void classifica(double **x, int N, int K, double **y, int nvet, int *c);
void centroide(double **x, int N, int K, double **y, int nvet, int *c);
double distorcao(double **x, int N, int K, double **y, int nvet, int *c);
double distq(double *x, double *y, int K);
void soma_vet(double *x, double *y, int K);

int main(){

    char dicion[cnome], dicioini[cnome], amos[cnome];
    int K, N, nvet, *c;
    double epslon, dist, dist_old;
    double **x, **y, aux;

    printf("Qual a dimensão do quantizador?\n");
    scanf("%d", &K);
    printf("Qual o número de níveis do dicionário?\n");
    scanf("%d", &N);
    printf("Qual o limite de redução percentual na distorção?\n");
    scanf("%lf", &epslon);
    printf("Qual o nome do arquivo que contém o dicionário inicial?\n");
    scanf("%s", dicioini);
    printf("Qual o nome do arquivo que contém a seqüência de treino?\n");
    scanf("%s", amos);
    printf("Qual o nome do arquivo para armazenar o dicionário final?\n");
    scanf("%s", dicion);

    /* Leitura dos vetores do dicionário inicial */

    x = ler_arquivo(dicioini,N,K);

    /* Leitura dos vetores de treino */

    y = ler_arquivo_treino(amos,&nvet,K);

    /* Alocação de memória para o vetor c que irá guardar para cada vetor 
       de treino y, o índice do vetor do dicionário que produz a menor
       distorção */

    c = aloca_vetor_int(nvet);

    /* O algoritmo LBG começa aqui */

    dist_old = 1e20;

    classifica(x, N, K, y, nvet, c);

    for(;;){

        centroide(x, N, K, y, nvet, c);

        dist = distorcao(x, N, K, y, nvet, c);

        aux = (dist_old - dist)/dist_old * 100;
        printf("distorção: %.3e \t redução(%%): %.3e\n",dist,aux);

        if(dist == 0.0){
            escreve_arquivo(dicion,x,N,K);
            exit(1);
        }

        if( (dist_old - dist)/dist_old < epslon/100.0 )
            break;

        dist_old = dist;
    }

    /* Escreve os vetores do dicionário calculado no arquivo de saída */

    escreve_arquivo(dicion,x,N,K);

    return (0);

} /* Fim do programa principal */


void classifica(double **x, int N, int K, double **y, int nvet, int *c){
    /******************************************************************
     * Esta função classifica os vetores de entrada y de acordo com os *
     * vetores do dicionário x, associando a variável c o índice do ve-*
     * tor que produz a menor distorção.                               *
     *    x = vetores do dicionário                                    *
     *    N = número de vetores do dicionário                          *
     *	  K = número de componentes de cada vetor                      *
     *	  y = vetores a serem classificados                            *
     *	  nvet = número de vetores a serem classificados               *
     *	  c = vetor que irá armazenar a classificação de cada vetor y  *
     ******************************************************************/

    int i, j;
    double dist;

    for(i=0;i<nvet;i++){
        dist = distq(y[i], x[0], K);
        c[i] = 0;
        for(j=1;j<N;j++){
            if( dist > distq(y[i], x[j], K) ){
                dist = distq(y[i], x[j], K);
                c[i] = j;
            }
        }
    }

    return;
}

double distq(double *x, double *y, int K){
    /******************************************************************
     * Esta função retorna a distância euclidiana quadrática entre os  *
     * vetores x e y, considerando K componentes em cada vetor         *
     ******************************************************************/

    double distq;
    int i;

    distq = 0.0;

    for(i=0;i<K;i++)
        distq += (x[i] - y[i]) * (x[i] - y[i]);

    return distq;
}

void centroide(double **x, int N, int K, double **y, int nvet, int *c){
    /******************************************************************
     * Esta função gera o dicionário de vetores x a partir dos vetores *
     * y  segundo a classificação armazenada na variável c             *
     * x = vetores do dicionário                                       *
     * N = número de vetores do dicionário                             *
     * K = número de componentes de cada vetor                         *
     * y = vetores de entrada                                          *
     * nvet = número de vetores y                                      *
     * c = vetor que armazena a classificação de cada vetor de y       *
     ******************************************************************/

    int i, j;
    int *nc; /* armazena o número de vetores de cada classe */
    double *temp, **x_aux;

    x_aux = aloca_matrizd(N,K);

    temp = aloca_vetor_double(K);

    nc = aloca_vetor_int(N);

    /* Inicia o processo de cálculo do centróide */
    /* 1.o Acumula os vetores */
    /* 2.o Normalização */

    for(i=0;i<nvet;i++)
        soma_vet( x_aux[c[i]], y[i], K);

    for(i=0;i<N;i++)
        nc[i] = 0;

    for(i=0;i<nvet;i++)
        nc[c[i]]++;

    for(i=0;i<N;i++)
        for(j=0;j<K;j++){
            temp[j] = x_aux[i][j];
            if(nc[i] != 0)
                x_aux[i][j] = temp[j]/nc[i];
        }

    for(i=0;i<N;i++)
        if(nc[i] != 0)
            for(j=0;j<K;j++)
                x[i][j] = x_aux[i][j];
    // Algortimo de Lee
    //		x[i][j] = x[i][j] + s *(x_aux[i][j] -x[i][j]);

    free(temp);
    free(nc);
    desaloca_matrizd(x_aux,N);

    return;
}

void soma_vet(double *x, double *y, int K){
    /******************************************************************
     * Esta função acrescenta o vetor y ao vetor x                     *
     ******************************************************************/

    int i;
    double temp[K];

    for(i=0;i<K;i++)
        temp[i] = x[i];

    for(i=0;i<K;i++)
        x[i] = temp[i] + y[i];

    return;
}

double distorcao(double **x, int N, int K, double **y, int nvet, int *c){
    /******************************************************************
     * Esta função calcula a distorção entre os vetores de entrada y e *
     * o dicionário calculado na função centróide.                     *
     ******************************************************************/

    int i;
    double dist_total;

    classifica(x, N, K, y, nvet, c);

    dist_total = 0.0;

    for(i=0;i<nvet;i++)
        dist_total += distq(y[i], x[c[i]], K);

    return dist_total;
}

