#include <iostream>
#include <math.h>
using namespace std;
int main(){
    int N, soma;

    cout << "Digite o valor de N (potência de 2): ";
    cin >> N;
    if(N&(N-1)) { cout << "N não é potência de 2\n"; exit(1);}

    int NU = (int)log2(N); // NU -> n.o de bits para representar N

    for(int L=0;L<NU;L++){
        cout << L+1 << ".o estágio\n"; 
        int LE = (int)pow(2,L+1);
        int LE1 = LE/2;
        for(int J=0;J<LE1;J++){
            for(int I=J;I<N;I+=LE){
                int IP = I + LE1;
                cout << I << " e " << IP << "\n";
            }
        }    
    }

    return(1);
}   
