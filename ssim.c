/******************************************************************
 *  Programa ssim.c                                                *
 *                                                                 *
 *  Calcula a métrica SSIM entre duas imagens no formato PGM (com  *
 *  cabeçalho) usando janelas 8x8 (variável block).                *
 *                                                                 *
 *  Z. Wang, A. C. Bovic, H. R. Sheikh and E. P. Simoncelli, "Ima- *
 *  ge Quality Assesment: From Error Visibility to Structural Si-  *
 *  milarity", IEEE Transactions On Image Processing, pp. 600-612  *
 *  Vol. 13, N.o 4, April 2004.                                    *
 *                                                                 *
 *                                                                 *
 *  Autor: Waslon Terllizzie Araújo Lopes                          *
 *  Data: 10/11/2016                                               *
 *                                                                 *
 *  Uso: ssim.out imagemA.pgm imagemB.pgm                          *
 ******************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include "alocacao.h"
#include "inout.h"
#define cnome 50 /* comprimento maximo de nome de arquivo */


double media_image(double **pta, int n, int m);
double std_dev_image(double **pta, int n, int m);
double covariance_image(double **pta, double **ptb, int n, int m);
double SSIM(double **pta, double **ptb, int n, int m);

int main (int argc, char **argv){

    (void)(argc); /* avoid unused parameter warning */
    int n,m,i,j,k,l,block;
    double **ptx, **pty, ssim, **pta, **ptb;
    char orig[cnome], quant[cnome];


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


    block = 8; //Calcula a SSIM em blocos 8x8

    pta = aloca_matrizd(block,block);
    ptb = aloca_matrizd(block,block);

    ssim = 0.0;
    for(i=0;i<n/block;i++)
        for(j=0;j<m/block;j++){
            for(k=0;k<block;k++) 
                for(l=0;l<block;l++){
                    pta[k][l] = ptx[block*i+k][block*j+l];
                    ptb[k][l] = pty[block*i+k][block*j+l];
                }

            ssim = ssim + SSIM(pta,ptb,block,block);
        }

    ssim=ssim*block*block/(n*m);

    printf("A SSIM entre as imagens é %lg\n",ssim);


    free (ptx);
    free (pty);
    free (pta);
    free (ptb);
    return 0;
}


double SSIM(double **ptx, double **pty, int n, int m){
    double mx, my, sigx, sigy, covxy,ssim, C1, C2;

    C1 = pow(0.01*255,2.0);
    C2 = pow(0.03*255,2.0);

    mx = media_image(ptx,n,m);
    my = media_image(pty,n,m);

    sigx = std_dev_image(ptx,n,m);
    sigy = std_dev_image(pty,n,m);

    covxy = covariance_image(ptx,pty,n,m);

    ssim = ((2*mx*my+C1)*(2*covxy+C2))/((pow(mx,2)+pow(my,2)+C1)*(pow(sigx,2)+pow(sigy,2)+C2));

    return(ssim);
}

double media_image(double **pta, int n, int m){
    double media;
    int i, j;

    media = 0.0;
    for(i=0;i<n;i++)
        for(j=0;j<m;j++)
            media+=pta[i][j];

    media = media/(n*m);

    return (media);
}


double std_dev_image(double **pta, int n, int m){
    double variance,media,std_dev; 
    int i, j;  

    media = media_image(pta,n,m);

    variance=0.0;
    for(i=0;i<n;i++)
        for(j=0;j<m;j++)
            variance+=pow(pta[i][j]-media,2.0);

    variance = variance/(n*m-1.0);

    std_dev = sqrt(variance);

    return (std_dev);
}


double covariance_image(double **pta, double **ptb, int n, int m){
    double covariance,mediax,mediay; 
    int i, j;  

    mediax = media_image(pta,n,m);
    mediay = media_image(ptb,n,m);

    covariance=0.0;
    for(i=0;i<n;i++)
        for(j=0;j<m;j++)
            covariance+=((pta[i][j]-mediax)*(ptb[i][j]-mediay));

    covariance = covariance/(n*m-1);

    return (covariance);
}
