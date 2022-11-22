#include <stdio.h>
#include <math.h>

#define Pi acos(-1)

#define MAX 8192

void main(void){

    char name[25];
    int i,npt;
    FILE *arquivo;
    float  *x, *y;

    x = (float *)malloc(MAX * sizeof(float));
    if(!x){printf("\nFalta de Memoria!");
        exit(1);
    }

    y = (float *)malloc(MAX * sizeof(float));
    if(!y){printf("\nFalta de Memoria!");
        exit(1);
    }

    printf("\nDigite o nome do arquivo de entrada: ");
    scanf("%s",&name);
    if((arquivo=fopen(name,"r"))==NULL){
        printf("\nNao foi possivel abrir o arquivo de entrada de dados\n");
        exit(1);
    }

    for(i=0;!feof(arquivo);i++)   //entrada de dados
        fscanf(arquivo,"%f %f",&x[i],&y[i]);
    fclose(arquivo);

    npt=i-1;



    printf("\nDigite o nome do arquivo de saida (modulo): ");
    scanf("%s",&name);
    if((arquivo=fopen(name,"w"))==NULL){
        printf("\nNao foi possivel abrir o arquivo de saida de dados\n");
        exit(1);
    }

    for(i=0;i<npt;i++)
        fprintf(arquivo,"%.10f\n",sqrt(x[i]*x[i]+y[i]*y[i]));
    fclose(arquivo);   


    printf("\nDigite o nome do arquivo de saida (fase): ");
    scanf("%s",&name);
    if((arquivo=fopen(name,"w"))==NULL){
        printf("\nNao foi possivel abrir o arquivo de saida de dados\n");
        exit(1);
    }

    for(i=0;i<npt;i++){
        if(x[i]*y[i])
            fprintf(arquivo,"%.10f\n",atan(y[i]/x[i]));
        else if(!y[i]){
            if(x[i]>=0.0)
                fprintf(arquivo,"%.10f\n", 0.0);
            else  
                fprintf(arquivo,"%.10f\n", -Pi);
        }
        else if(!x[i]){
            if(y[i]>=0.0)
                fprintf(arquivo,"%.10f\n", Pi/2.0);
            else  
                fprintf(arquivo,"%.10f\n", -Pi/2.0);
        }
    }  
    fclose(arquivo);   

}
