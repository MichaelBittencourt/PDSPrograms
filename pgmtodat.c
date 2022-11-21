/* Programa de conversao de imagens *.pgm
   para imagens no formato *.dat(madeiro)

Autor: Waslon Terllizzie Araujo Lopes
Data: Dez/99   
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <malloc.h>
#include <math.h>

#define DIM 256
int main(void){

    char name[25];
    int i,j,k,l;
    FILE *arquivo;
    int *ha,**pta;
    float x;

    printf("\nDigite o nome do arquivo  de entrada de dados (*.pgm): ");
    scanf("%s",&name[0]);
    if((arquivo=fopen(name,"r"))==NULL){
        printf("\nNao foi possivel abrir o arquivo de entrada de dados\n");
        exit(1);
    }


    // alocacao da memoria 

    ha = (int *)malloc(DIM * DIM  * sizeof(int)); //n.o de pixels para niv. de cinza

    if( (!ha) ){
        printf("\nFalha na alocacao de memoria\n");
        exit(1);
    }

    // alocacao da memoria  -- imagem de entrada

    pta = (int **)malloc(DIM*sizeof(int *));  // nivel de cinza de cada pixel
    if(!pta){
        printf("\nFalha na alocacao de memoria\n");
        exit(1);
    }

    for(i=0;i<DIM;i++){
        pta[i] =(int *)malloc(DIM * sizeof(int));
        if(!pta[i]) {
            printf("\nFalha na alocacao de memoria\n");
            exit(1);
        }
    }

    //leitura dos valores da matriz da imagem de entrada

    //Primeiro elimina-se o cabeçalho

    fscanf(arquivo,"%s %d %d %d",&name[0], &i, &i, &i);

    //leitura propriamente dita

    for(i=0;i<DIM;i++)
        for(j=0;j<DIM;j++){
            fscanf(arquivo,"%f",&x);
            *(pta[i]+j) = x;
        }
    fclose(arquivo);


    // construcao do vetor  a partir da imagem  de entrada

    for(i=0;i<64;i++)
        for(j=0;j<64;j++)
            for(k=0;k<4;k++)
                for(l=0;l<4;l++)
                    ha[1024*i + 16*j + 4*k + l] = pta[4*i+k][4*j+l];

    //armazenamento da imagem reconstruida em arquivo

    printf("\nDigite o nome do arquivo da imagem de saída (*.dat): ");
    scanf("%s",&name[0]);
    if((arquivo=fopen(name,"w"))==NULL){
        printf("\nNao foi possivel abrir o arquivo de saida de dados\n");
        exit(1);
    }
    for(i=0;i<DIM*DIM;i++)
        fprintf(arquivo,"%d\n",*(ha + i));
    fclose(arquivo);

    return 1;
}     // fim do programa principal
