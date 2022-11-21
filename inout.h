#ifndef	_INOUT_H
#define	_INOUT_H	1

#include"alocacao.h"

double **ler_arquivo(char nome[], int N, int K);
double **ler_arquivo_pgm(char nome[], int *N, int *K);
double **ler_arquivo_treino(char nome[], int *nvet, int K);
double *ler_vetor_arquivo_treino(char nome[], int *nvet);
void escreve_arquivo(char nome[], double **x, int N, int K);
void escreve_vetor_arquivo(char nome[], double *x, int N);
void escreve_arquivo_lsf(char nome[], double **x, int N, int K);

double *ler_vetor_arquivo_treino(char nome[], int *nvet){
    /*******************************************************************
     * Função que ler retorna um ponteiro de dimensão nvet,K com os veto*
     * res lidos a partir do dicionário armazenado no arquivo "nome".   *
     * A função também retorna o número de vetores k-dimensionais do ar *
     * quivo de treino através da variável N.                           *
     *******************************************************************/ 
    double **x,*y;
    int i;

    x = ler_arquivo_treino(nome,nvet,1);

    y = aloca_vetor_double(*nvet);

    for(i=0;i<*nvet;i++)
        y[i] = x[i][0];

    free(x);
    return(y);
}


double **ler_arquivo(char nome[], int N, int K){
    /*******************************************************************
     * Função que ler retorna um ponteiro de dimensão N x K com os veto-*
     * res lidos a partir do dicionário armazenado no arquivo "nome"    *
     *******************************************************************/ 

    FILE *arquivo;
    int i, j;
    double **x;

    /* Leitura dos vetores do dicionário a partir do arquivo */

    arquivo = fopen(nome, "r");
    if (!arquivo) {
        printf("\nErro na abertura do arquivo %s!!!\a\n",nome);
        exit(1);
    }

    /* Alocação de memória para o vetor */

    x = aloca_matrizd(N,K);

    /* Leitura propriamente dita */

    for(i=0;i<N;i++)
        for(j=0;j<K;j++)
            fscanf(arquivo,"%lf",(x[i]+j));
    fclose(arquivo);

    return (x);
}


double **ler_arquivo_pgm(char nome[], int *N, int *K){
    /*******************************************************************
     * Função que ler retorna um ponteiro de dimensão N x K com os veto-*
     * res lidos a partir do dicionário armazenado no arquivo "nome"    *
     *******************************************************************/ 

    FILE *arquivo;
    int i, j;
    double **x;

    /* Leitura dos vetores do dicionário a partir do arquivo */

    arquivo = fopen(nome, "r");
    if (!arquivo) {
        printf("\nErro na abertura do arquivo %s!!!\a\n",nome);
        exit(1);
    }

    /* Eliminação do cabeçalho */  

    fscanf(arquivo,"%s %d %d %s", nome, N, K, nome);
    /* Alocação de memória para o vetor */

    x = aloca_matrizd(*N,*K);

    /* Leitura propriamente dita */

    for(i=0;i<*N;i++)
        for(j=0;j<*K;j++)
            fscanf(arquivo,"%lf",(x[i]+j));
    fclose(arquivo);

    return (x);
}



double **ler_arquivo_treino(char nome[], int *nvet, int K){
    /*******************************************************************
     * Função que ler retorna um ponteiro de dimensão nvet,K com os veto*
     * res lidos a partir do dicionário armazenado no arquivo "nome".   *
     * A função também retorna o número de vetores k-dimensionais do ar *
     * quivo de treino através da variável N.                           *
     *******************************************************************/ 

    FILE *arquivo;
    int i, j;
    double **y,aux;

    /* Leitura dos vetores do dicionário a partir do arquivo */

    arquivo = fopen(nome, "r");
    if (!arquivo) {
        printf("\nErro na abertura do arquivo %s!!!\a\n",nome);
        exit(1);
    }

    /* Leitura dos vetores de treino:  
       1.o Passo) Faz-se a leitura do arquivo, só para contar o número de
       vetores necessários;
       2.o Passo) Aloca-se então memória para os vetores;
       3.o Passo) Faz-se a leitura dos vetores
       */

    /* 1.o Passo */

    *nvet = 0;
    for(i=1;!feof(arquivo);i++){
        fscanf(arquivo,"%lf",&aux);
        if(feof(arquivo)) break;	 
        for(j=1;j<K;j++)
            fscanf(arquivo,"%lf",&aux);
        *nvet = i;
    }

    rewind(arquivo); /* Retorna o ponteiro para o início do arquivo */

    /* 2.o Passo */

    /* Alocação de memória para os vetores de treino */
    /* nvet vetores de dimensão K */

    y = aloca_matrizd(*nvet,K); 

    /* 3.o Passo */

    for(i=0;i<*nvet;i++)
        for(j=0;j<K;j++)
            fscanf(arquivo,"%lf",(y[i]+j));
    fclose(arquivo);

    return (y);
}

void escreve_arquivo(char nome[], double **x, int N, int K){
    /*******************************************************************
     * Função que escreve os N vetores K-dimensionais armazenados no    *
     * ponteiro x no arquivo "nome".                                    *
     *******************************************************************/ 

    FILE *arquivo;
    int i, j;

    arquivo = fopen(nome, "w");
    if (!arquivo) {
        printf("\nErro na abertura do arquivo %s!!!\a\n",nome);
        exit(1);
    }

    for(i=0;i<N;i++)
        for(j=0;j<K;j++)
            fprintf(arquivo,"%f\n",x[i][j]);

    fclose(arquivo);

    return;

}


void escreve_vetor_arquivo(char nome[], double *x, int N){
    /*******************************************************************
     * Função que escreve os N vetores K-dimensionais armazenados no    *
     * ponteiro x no arquivo "nome".                                    *
     *******************************************************************/ 

    FILE *arquivo;
    int i;

    arquivo = fopen(nome, "w");
    if (!arquivo) {
        printf("\nErro na abertura do arquivo %s!!!\a\n",nome);
        exit(1);
    }

    for(i=0;i<N;i++)
        fprintf(arquivo,"%.10f\n",x[i]);

    fclose(arquivo);

    return;

}
void escreve_arquivo_lsf(char nome[], double **x, int N, int K){
    /*******************************************************************
     * Função que escreve os N vetores K-dimensionais armazenados no    *
     * ponteiro x no arquivo "nome".                                    *
     *******************************************************************/ 

    FILE *arquivo;
    int i, j;

    arquivo = fopen(nome, "w");
    if (!arquivo) {
        printf("\nErro na abertura do arquivo %s!!!\a\n",nome);
        exit(1);
    }

    for(i=0;i<N;i++)
        for(j=0;j<K;j++)
            fprintf(arquivo,"%.14f\n",x[i][j]);

    fclose(arquivo);

    return;

}

#endif	/* inout.h  */
