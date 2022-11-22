/* Programa para calculo da  transformada rapida de Fourier
   direta e inversa usando decimacao no tempo e na frequencia

Autor: Waslon Terllizzie
Data: ago/98                                  
*/   
#include "./complex.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <malloc.h>
#include "fft.h"

#define MAX 8192

void bit_reverse( fcomplex *pt, int N);       //reversao de bits
void t_fft( fcomplex *pt, int N, float sign); //transformada de fourier - dec. no tempo 
void f_fft( fcomplex *pt, int N, float sign); //transformada de fourier - dec. na freq.

int  main(void){

    char name[25];
    int i,N,opt,j;
    FILE *arquivo;
    fcomplex *pt;

    if(!(pt=malloc(MAX*sizeof(fcomplex)))){  //ALocacao de Memoria
        printf("\n Sem Memoria\n");
        exit(1);}

    printf("\nDigite o nome do arquivo de entrada: ");
    scanf("%s",&name);
    if((arquivo=fopen(name,"r"))==NULL){
        printf("\nNao foi possivel abrir o arquivo de entrada de dados\n");
        exit(1);
    }


    for(i=0;!feof(arquivo);i++)   //entrada de dados
        fscanf(arquivo,"%f %f\n",&(pt+i)->r,&(pt+i)->i);
    fclose(arquivo);

    N=i;                           //numero de amostras obtidas 


    {//verificacao se n.o de pts de entrada eh pot de dois
        j = N;
        opt = 0;                                
        for(i=0;i<(int)(log(pow(MAX,2))/log(2));i++){ 
            opt += j&1;                      
            j = j/2;
        }
        if(opt!=1){
            printf("\nDados invalidos");
            printf("\nO numero de pontos do arquivo de entrada tem que ser potencia de 2\n");
            exit(1);}
    }


    /*Escolha do Tipo de Transformada*/ 

    printf("\n  Tipo de Transformada\n");

    printf("\nDireta com dec. no tempo\t (1)");
    printf("\nDireta com dec. em freq.\t (2)");
    printf("\nInversa com dec. no tempo\t (3)");
    printf("\nInversa com dec. em freq.\t (4)");
    printf("\nReversao de bits \t \t (5)");

    printf("\nDigite a sua opcao: ");

    scanf("%d",&opt);
    switch(opt)
    {
        case 1:
            { 
                bit_reverse(pt,N); 
                t_fft(pt,N,1);
                break;  
            }
        case 2:
            {
                f_fft(pt,N,1);
                bit_reverse(pt,N); 
                break;
            }
        case 3:
            {
                bit_reverse(pt,N); 
                t_fft(pt,N,-1);
                break;
            }
        case 4:
            {
                f_fft(pt,N,-1);
                bit_reverse(pt,N); 
                break;
            }
        case 5:
            {
                bit_reverse(pt,N);
                break;
            }
        default:
            exit(1);
    }  

    //for(i=0;i<N;i++)                  //impressao na tela
    //printf("\t%f\t%f\n",(pt+i)->r,(pt+i)->i);

    printf("\nDigite o nome do arquivo de saida: ");
    scanf("%s",&name);
    if((arquivo=fopen(name,"w"))==NULL){
        printf("\nNao foi possivel abrir o arquivo de saida de dados\n");
        exit(1);
    }

    for(i=0;i<N;i++)
        fprintf(arquivo,"%f \t%f\n",(pt+i)->r,(pt+i)->i);
    fclose(arquivo);

    free(pt);

    return 1;
}
