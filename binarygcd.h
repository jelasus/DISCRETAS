#include<NTL/ZZ.h>
using namespace NTL;
typedef ZZ num;
num gcd(num a, num b){
  while (a!=b){
    if (a>b)
      a=a-b;
    else
      b=b-a;
  }
  return a;
}
ZZ div2(ZZ a){
    return a >> 1;
}

ZZ binaryGCD(ZZ x, ZZ y){
    int g=1;
    ZZ a;
    ZZ t;
    ZZ resultx,resulty;
    t=1;
    a=2;
    while (x!=0){
            bit_and(resultx,x,t);
            bit_and(resulty,y,t);

        if( (resultx==0) && (resulty==0)){
          x=div2(x); y=div2(y); g=g*2;
        }
        if ((resultx==0) && (resulty==1)){
            x=div2(x);
        }
        if ((resultx==1) && (resulty==0)){
            y=div2(y);
        }
        if ((resultx==1)&&(resulty==1)) {
            if (x>=y){
                x=div2(x-y);
            }
            else{
                y=div2(y-x);
            }
        }
    }
    return g*y;
}
