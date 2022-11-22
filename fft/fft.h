/* Programa para calculo da  transformada rapida de Fourier
   direta e inversa usando decimacao no tempo e na frequencia

Autor: Waslon Terllizzie
Data: ago/98                                  
Neste arquivo se encontram somente as funcoes utilizadas pelo
programa fft.c
*/   
#include <math.h>


void bit_reverse( fcomplex *pt, int N);       //reversao de bits
void t_fft( fcomplex *pt, int N, float sign); //transformada de fourier - dec. no tempo 
void f_fft( fcomplex *pt, int N, float sign); //transformada de fourier - dec. na freq.

/* Nao esqueca de chamar a rotina bit_reverse antes de fazer a TF com decimacao no tempo e apos a decimacao em frequencia */


void bit_reverse( fcomplex *pt, int N){

    //Efetua a reversao dos bits

    int i,j,soma,NU;
    fcomplex *y;

    if(!(y=malloc(N*sizeof(fcomplex)))){  //ALocacao de Memoria
        printf("\n Sem Memoria\n");
        exit(1);}

    NU = (int) ((float)log(N)/log(2));

    for(i=0;i<N;i++)
        *(y+i)=*(pt+i);

    for(i=0;i<N;i++){ // a reversao ocorre aqui!!
        soma = 0;
        for(j=0;j<NU;j++)
            if ((int)pow(2,j)&i) soma = soma + (int)pow(2,NU-1-j);
        *(pt+soma)=*(y+i);
    }
    free(y);
    return ;
}   

//-------------------------------------------------------------------

void t_fft( fcomplex *pt, int N, float  sign){
    //Efetua a Transformada Rapida de Fourier (FFT) com decimacao no tempo
    // sign = 1 -> transf direta   sign = -1 -> transf inversa

    int I,J,L,NU,IP,LE,LE1;
    fcomplex u,w,temp;



    NU = (int) ((float)log(N)/log(2));

    for(L=1;L<(NU+1);L++){ // Número de estágios
        LE = (int)pow(2,L);
        LE1 = LE/2;
        u = (fcomplex){1.0,0.0};
        w = Wn(LE,sign);
        for(J=1;J<(LE1+1);J++){
            for(I=J;I<(N+1);I+=LE){
                IP = I + LE1;
                temp = Cmul(*(pt+(IP-1)),u);
                *(pt+(IP-1)) = Csub(*(pt+(I-1)),temp);
                *(pt+(I-1)) = Cadd(*(pt+(I-1)),temp);
            }
            u = Cmul(u,w);
        }    
    }
    if(sign==-1)
        for(I=0;I<N;I++)
            *(pt + I) = RCmul((1.0/N),*(pt + I));
    return;
}

//-------------------------------------------------------------------

void f_fft( fcomplex *pt, int N, float sign){
    //Efetua a Transformada Rapida de Fourier (FFT) com decimacao em freq.
    // sign = 1 -> transf direta   sign = -1 -> transf inversa
    //sem efetuar a revesao de bits

    int I,J,L,NU,IP,LE,LE1;
    fcomplex u,w,temp;

    NU = (int) ((float)log(N)/log(2));

    for(L=1;L<(NU+1);L++){
        LE = (int)pow(2,NU+1-L);
        LE1 = LE/2;
        u = (fcomplex){1.0,0.0};
        w = Wn(LE,sign);
        for(J=1;J<(LE1+1);J++){
            for(I=J;I<(N+1);I+=LE){
                IP = I + LE1;
                temp = Cadd(*(pt+(I-1)),*(pt+(IP-1)));
                *(pt+(IP-1)) = Cmul(u,Csub(*(pt+(I-1)),*(pt+(IP-1))));
                *(pt+(I-1)) = temp;
            }
            u = Cmul(u,w);
        }    
    }
    if(sign==-1)
        for(I=0;I<N;I++)
            *(pt + I) = RCmul((1.0/N),*(pt + I));

    return;
}
