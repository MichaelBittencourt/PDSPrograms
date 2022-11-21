/* Programa de conversao de imagens *.dat(madeiro)
   para imagens no formato *.pgm

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
    long w;
    float x;

    printf("\nDigite o nome do arquivo  de entrada de dados (*.dat): ");
    scanf("%s",&name[0]);
    if((arquivo=fopen(name,"r"))==NULL){
        printf("\nNão foi possível abrir o arquivo de entrada de dados\n");
        exit(1);
    }


    // alocacao da memoria 

    ha = (int *)malloc(DIM * DIM  * sizeof(int)); //n.o de pixels para niv. de cinza

    if( (!ha) ){
        printf("\nFalha na alocação de memória\n");
        exit(1);
    }



    // alocacao da memoria  -- imagem de entrada

    pta = (int **)malloc(DIM*sizeof(int *));  // nivel de cinza de cada pixel
    if(!pta){
        printf("\nFalha na alocação de memória\n");
        exit(1);
    }

    for(i=0;i<DIM;i++){
        pta[i] =(int *)malloc(DIM * sizeof(int));
        if(!pta[i]) {
            printf("\nFalha na alocação de memória\n");
            exit(1);
        }
    }



    //leitura dos valores da matriz da imagem de entrada

    for(i=0;i<DIM*DIM;i++){
        fscanf(arquivo,"%f",&x);
        *(ha+i) = x;
    }
    fclose(arquivo);


    // reconstrucao da imagem a partir do vetor de entrada

    for(i=0;i<64;i++)
        for(j=0;j<64;j++)
            for(k=0;k<4;k++)
                for(l=0;l<4;l++)
                    pta[4*i+k][4*j+l] = ha[1024*i + 16*j + 4*k + l];

    //armazenamento da imagem reconstruida em arquivo

    printf("\nDigite o nome do arquivo da imagem de saida (*.pgm): ");
    scanf("%s",&name[0]);
    if((arquivo=fopen(name,"w"))==NULL){
        printf("\nNão foi possível abrir o arquivo de saída de dados\n");
        exit(1);
    }

    fprintf(arquivo,"P2 \n256 256 \n255\n"); //Cabecalho .pgm

    w = 1;
    for(i=0;i<DIM;i++)
        for(j=0;j<DIM;j++){
            fprintf(arquivo,"%d ",*(pta[i]+j));
            if(!(w%256))
                fprintf(arquivo,"\n");
            w++;
        }
    fclose(arquivo);

    return 1;
}     // fim do programa principal
