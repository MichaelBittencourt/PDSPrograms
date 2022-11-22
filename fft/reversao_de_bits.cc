#include <iostream>
#include <math.h>
using namespace std;
int main(){
    int N, soma;

    cout << "Digite o valor de N (potência de 2): ";
    cin >> N;
    if(N&(N-1)) {cout << N << " não é potência de 2\n"; exit(1);}

    //int NU = (int) ((float)log(N)/log(2)); // NU -> n.o de bits para representar N
    int NU = (int)log2(N); // NU -> n.o de bits para representar N

    for(int i=0;i<N;i++){ 
        soma = 0;
        for(int j=0;j<NU;j++)
            if ((int)pow(2,j)&i) soma = soma + (int)pow(2,NU-1-j);
        cout <<"   " <<  soma << "\n";
    }
    return(1);
}   
