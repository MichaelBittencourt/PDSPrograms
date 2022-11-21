#ifndef	_COMPLEX_H
#define	_COMPLEX_H	1


/* Programa para implementacao das funcoes complexas */

#include <math.h>

#define Pi acos(-1)

typedef struct FCOMPLEX { double r,i;} fcomplex;

fcomplex Cadd( fcomplex a, fcomplex b);
fcomplex Cmul( fcomplex a, fcomplex b);
fcomplex Csub( fcomplex a, fcomplex b);
fcomplex Cdiv( fcomplex a, fcomplex b);
fcomplex Complex( double re, double im);
fcomplex Conjg( fcomplex z);
double Cabs( fcomplex z);
fcomplex Csqrt( fcomplex z);
fcomplex RCmul( double x, fcomplex a);
fcomplex Cpot(fcomplex a, int e);
fcomplex Wn( int N, double sign);
fcomplex Cexp(double sign);


fcomplex Cexp(double a){
    // Retorna exp(j * a)
    fcomplex c;

    c.r = cos(a);
    c.i = sin(a);

    return (c);
}

fcomplex Cadd( fcomplex a, fcomplex b)
{
    // Retorna a soma complexa de dois n.os cplxs
    fcomplex c;
    c.r = a.r + b.r;
    c.i = a.i + b.i;
    return c;
}

fcomplex Csub( fcomplex a, fcomplex b)
{
    // Retorna a sub. complexa de dois n.os cplxs
    fcomplex c;
    c.r = a.r - b.r;
    c.i = a.i - b.i;
    return c;
}

fcomplex Cmul( fcomplex a, fcomplex b)
{
    // Multiplicacao cplx de dois n.os cplx
    fcomplex c;
    c.r = a.r*b.r-a.i*b.i;
    c.i = a.i*b.r+a.r*b.i;
    return c;
}

fcomplex Cdiv( fcomplex a, fcomplex b)
{
    // Divisao de dois n.os cplx
    fcomplex c;
    double r,den;
    if (fabs(b.r)>= fabs(b.i)){
        r= b.i/b.r;
        den = b.r + r*b.i;
        c.r = (a.r + r*a.i)/den;
        c.i = (a.i - r*a.r)/den;
    }  else {
        r = b.r/b.i;
        den = b.i + r*b.r;
        c.r = (a.r*r + a.i)/den;
        c.i = (a.i*r - a.r)/den;
    }
    return c;
} 

fcomplex Complex( double re, double im)
{
    // Retorna um cplx com as partes real=a e imag.=b
    fcomplex c;
    c.r = re;
    c.i = im;
    return c;
}


double Cabs( fcomplex z){
    // Retorna o modulo do n.o cplx
    double x,y,ans,temp;
    x=fabs(z.r);
    y=fabs(z.i);
    if (x==0.0)
        ans = y;
    else if ( y==0.0)
        ans = x;
    else if(x>y){
        temp=y/x;
        ans=x*sqrt(1.0+temp*temp);
    } else {
        temp = x/y;
        ans = y*sqrt(1.0+temp*temp);
    }
    return ans;
}

fcomplex Conjg( fcomplex z){
    // Retorna o cplx conjugado de z
    fcomplex c;
    c.r=z.r;
    c.i=-z.i;
    return c;
}

fcomplex Csqrt( fcomplex z){
    //Retorna a raiz quad. de um n.o cplx
    fcomplex c;
    double x,y,w,r;
    if ((z.r==0.0) && (z.i == 0.0)){
        c.r=c.i=0.0;
        return c;
    }  else {
        x=fabs(z.r);
        y=fabs(z.i);
        if (x>=y) {
            r=y/x;
            w=sqrt(x)*sqrt(0.5*(1.0+sqrt(1.0+r*r)));
        }  else {
            r=x/y;
            w=sqrt(y)*sqrt(0.5*(r+sqrt(1.0+r*r)));
        }
        if (z.r>= 0.0){
            c.r=w;
            c.i=z.i/(2.0*w);
        } else {
            c.i=(z.i>=0) ? w : -w;
            c.r=z.i/(2.0*c.i);
        }
        return c;
    }
}   

fcomplex RCmul( double x, fcomplex a){
    // Retorna o produto real * cplx
    fcomplex c;
    c.r=x*a.r;
    c.i=x*a.i;
    return c;
}

fcomplex Cpot(fcomplex a, int e)
{
    fcomplex temp; 
    temp.r=1.0;
    temp.i=0.0;
    if(!e) return temp;
    else
        for (;e;e--) temp = Cmul(temp,a);
    return temp;
}

fcomplex Wn(int N, double sign)
{// sign = 1 -> cos(2*Pi/N) - j cos(2*Pi/N)
 // sign = -1 -> cos(2*Pi/N) + j cos(2*Pi/N)
    fcomplex c;
    // double Pi=3.14159265359;
    if(N){
        c.r= cos(2*Pi/N);
        c.i= -sin(2*Pi/N)*sign;
    } else {
        c.r=1.0;
        c.i=0.0;
    }
    return c;
}

#endif	/* complex.h  */
