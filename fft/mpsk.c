/******************************************************************
 * Faz a transmissao nao codificada de sinais MPSK em canal com    *
 * desvanecimento Rayleigh e ruído gaussiano branco.               *
 ******************************************************************/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "rand.h"
#include "complex.h"
#include "alocacao.h"

//typedef  struct{unsigned bit : 1;} sbit;

double mpsk(double Eb_No, int M, double theta, int K, int MaxErros);
double distancia(fcomplex r1, int i, int j, fcomplex *psk, fcomplex *alfa);
fcomplex *gera_constelacao(int M, double angulo);
fcomplex *normaliza_constelacao(fcomplex *psk, int M);
int *gray(int N);
int n_bits(int NPT);


int main(){
    int  ErrosMax, m,  M, K;
    double media, theta;
    FILE *fp;
    time_t start, end;
    char nome[30];

    reset_rand(); //reseta o gerador de n.os aleatorios

    start = time(NULL); //inicio da contagem de tempo

    printf("Digite o nome do arquivo de saída de dados: \n");
    scanf("%s",&nome[0]);
    if( !(fp = fopen(nome,"a"))) exit(0);

    printf("Digite a dimensão da constelação (M): \n");
    scanf("%d",&M);

    printf("Digite o ângulo de rotação constelação (Theta): \n");
    scanf("%lf",&theta);

    printf("Digite o tamanho de blocos de símbolos a serem transmitidos (K): \n");
    scanf("%d",&K);

    printf("Digite o número de erros necessários para o cálculo de Pb: \n");
    scanf("%d",&ErrosMax);

    printf("Simulando...\n");
    fprintf(fp,"\n");

    for(m=0;m<=30;m+=2){
        media = mpsk(m,M,theta,K,ErrosMax);
        printf("Eb/No = %.2f dB \t Prob. de erro (bit) = %.2e\n",1.0*m,media);
        fprintf(fp," %.2f \t  %.3e\n",1.0*m,media);
        fflush(fp);
    }
    end = time(NULL);
    printf("Tempo de execucao do programa:  %.1f segundos\n",difftime(end,start));
    fprintf(fp,"#Tempo de execucao do programa:  %.1f segundos\n",difftime(end,start));
    printf("\nPrograma encerrado com sucesso!\a\n\a\a\a\n");
    fclose(fp);
    return 1;
}

double mpsk(double Eb_No, int M, double theta, int K, int MaxErros){
    /******************************************************************
     * Faz a transmissão dos sinais da constelação MPSK através de um  *
     * canal com desvanecimento rayleigh.                              *
     * Usa-se o entrelaçamento dos sinais para se obervar o            * 
     *                                                                 *
     * Eb_No = Energia de Bit/Densidade de Ruido (em dB)               *
     * theta = ângulo de rotação da constelação                        *
     * M = número de sinais da constelação                             *
     * K = tamanho do bloco                                            *
     * MaxErros = Maior número de erros para obter Pb                  *
     *                                                                 *
     * A saída da função é a probabilidade de erro do sistema, calcula *
     * da quando o número de erros é maior ou igual a MaxErros         *
     *                                                                 *
     ******************************************************************/ 
    int i,j,*x,l,NoErros,TotalDeBits, BitsPorSimbolo, TotalDeSimbolos, NumeroDeBlocos,NoErrosTotal;
    sbit *tx, *receiv;
    double No,dist,SomaDasMedias,MediaNoBloco;
    fcomplex *psk, *alfa, *s, *d; 

    // Geração da constelação

    psk = gera_constelacao(M,theta);

    // Normalização da constelação

    psk = normaliza_constelacao(psk,M);

    BitsPorSimbolo = n_bits(M);

    TotalDeSimbolos = K;

    TotalDeBits = BitsPorSimbolo * TotalDeSimbolos;

    // Alocação de memória

    alfa = aloca_vetor_fcomplex(TotalDeSimbolos);
    tx = aloca_vetor_sbit(TotalDeBits);
    x = aloca_vetor_int(TotalDeSimbolos);
    s = aloca_vetor_fcomplex(TotalDeSimbolos);
    d = aloca_vetor_fcomplex(TotalDeSimbolos);
    receiv = aloca_vetor_sbit(TotalDeBits);

    No = 1.0/BitsPorSimbolo * pow(10,-Eb_No/10);

    //No = 0.00;

    NumeroDeBlocos = 0;
    SomaDasMedias = 0.0;
    NoErrosTotal = 0;
    NoErros = 0;

    while(NoErrosTotal <= MaxErros){

        for(i=0;i<TotalDeSimbolos;i++)
            alfa[i] = (fcomplex){gauss(0,0.5),gauss(0,0.5)};

        // Gera bits para serem transmitidos

        for(i=0;i<TotalDeBits;i++)
            if (gera_bits(0.5))
                tx[i].bit = 1;
            else tx[i].bit = 0;

        // conversao bit para sinal  ( x = 2 * tx[1].bit + tx[0].bit )

        for(i=0;i<TotalDeSimbolos;i++)
            x[i] = 0.0;

        for(i=0;i<TotalDeSimbolos;i++)
            for(j=0;j<BitsPorSimbolo;j++)
                x[i] += (tx[BitsPorSimbolo*i+j].bit * (int)pow(2,j));

        // geracao do sinal QPSK

        for(i=0;i<TotalDeSimbolos;i++)
            s[i] = psk[x[i]];

        //Transmissao do sinal

        for(i=0;i<TotalDeSimbolos;i++)
            d[i] = Cadd(Cmul(alfa[i],s[i]),(fcomplex){gauss(0,No/2),gauss(0,No/2)});


        // Decodifica o sinal recebido - atraves do criterio de menor distancia
        // Armazena a seqüência de símbolos no vetor x[].

        for(i=0;i<TotalDeSimbolos;i++){
            dist = distancia(d[i],i,0,psk,alfa);
            j=0;
            for(l=1;l<M;l++)
                if(dist > distancia(d[i],i,l,psk,alfa)){
                    dist = distancia(d[i],i,l,psk,alfa);
                    j=l;
                }
            x[i] = j;
        }

        // Obtem os bits a partir do sinal recebido

        for(i=0;i<TotalDeSimbolos;i++)  
            for(j=0;j<BitsPorSimbolo;j++)
                if( ((int)pow(2,j)) & x[i] )
                    receiv[BitsPorSimbolo*i+j].bit = 1;
                else
                    receiv[BitsPorSimbolo*i+j].bit = 0;

        //Conta os erros 

        NoErros = 0;

        for(i=0;i<TotalDeBits;i++)
            if(tx[i].bit != receiv[i].bit){
                NoErros++; 
                NoErrosTotal++;
            }

        MediaNoBloco = (1.0*NoErros)/(1.0*TotalDeBits);
        //printf("%d\n",NoErros);
        //printf("%e\n",MediaNoBloco);

        NumeroDeBlocos++;

        SomaDasMedias += MediaNoBloco;

    }

    // Liberação de memória 

    free(x);
    free(tx); 
    free(receiv);
    free(alfa);
    free(s);
    free(d); 

    return (SomaDasMedias/NumeroDeBlocos);
}

double distancia( fcomplex r1, int i, int j, fcomplex *psk, fcomplex *alfa){ 
    //Funcao que calcula a distancia entre o sinal recebido e os sinais da constelacao

    double m;
    fcomplex temp;

    temp = Csub(r1,Cmul(alfa[i],psk[j]));
    m = Cabs(temp);
    return (m*m);
}

fcomplex *gera_constelacao(int M, double Theta){
    /******************************************************************
     * Função que gera uma constelação MPSK com M sinais.              *
     * Usa-se o mapeamento Gray na atribuição dos índices.             *
     * Rotaciona a constelação de Theta graus.                         *
     ******************************************************************/

    fcomplex *mpsk ;
    int *vetor_gray;
    int i;

    mpsk = (fcomplex *)malloc(M*sizeof(fcomplex));  
    if(!mpsk){
        printf("\nFalha na alocação de memória\n");
        exit(1);
    }

    vetor_gray = gray(M);

    for(i=0;i<M;i++)
        mpsk[ vetor_gray[i] ] = (fcomplex){cos(2*Pi/M * i) ,sin(2*Pi/M * i)}; 

    for(i=0;i<M;i++)
        mpsk[i] = Cmul(mpsk[i],(fcomplex){cos(Theta*Pi/180) ,sin(Theta*Pi/180)}); 

    return (mpsk);

}

int *gray(int N){
    /******************************************************************
     * Função que retorna um vetor inteiro com mapeamento Gray.        *
     ******************************************************************/

    int bits, cont, x, i, j, *vetor,**pta;

    bits = n_bits(N);

    //Alocação de memória 

    vetor = aloca_vetor_int(N);

    pta = aloca_matriz_int(N,bits);

    for(i=0;i<bits;i++)
    {
        for(j=0;j<(int)pow(2,i);j++)
            *(pta[j]+i) = 0;
        cont = 1;
        x = 0;
        for(j=(int)pow(2,i);j<N;j++){
            *(pta[j]+i) = cont;
            x++;
            if(x==(int)pow(2,i+1))
                x = 0;
            if(x==0)
                cont = (int)abs(1-cont);
        }
    }

    for(i=0;i<N;i++){
        cont = 0;
        for(j=0;j<bits;j++)
            cont += *(pta[i]+j) * pow(2,j);
        *(vetor + i) = cont;
    }

    return(vetor);
}

int n_bits(int NPT){
    //Determina o número de bits para representar o inteiro NPT
    int i, bits;

    for(i=1;i<20;i++)
        if((NPT&((int)pow(2,i)))>>i)
            bits = i;
    //printf("bits = %d",bits);

    return bits;
}

fcomplex *normaliza_constelacao(fcomplex *psk, int M){
    /******************************************************************
     * Normaliza a constelação.                                        *
     ******************************************************************/

    int i, j;

    double energia_total;

    energia_total = 0;

    for(i=0;i<M;i++)
        for(j=0;j<M;j++)
            energia_total += Cabs((fcomplex){psk[i].r,psk[j].i});

    energia_total /= (M*M);	 

    for(i=0;i<M;i++)
        psk[i] = RCmul(1.0/energia_total,psk[i]);

    return (psk);
}
