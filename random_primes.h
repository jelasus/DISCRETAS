#include <iostream>
#include <NTL/ZZ.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
//#define MINBITS 128
using namespace NTL;
typedef ZZ num;

num power(num base, num  exp, num modulo){
  num result;//resultado final, empieza en uno porque el primer resultado es igual a la base%modulo
  result=1;
  while(exp>0){
    base=base%modulo; //el numero que se enviara a multiplicar en la siguiente operacion de exponenciacion
    if ((exp & 1)==1){//si el exponente es impar..
      result=(base*result)%modulo;//se hace el resultado de la correspondiente exponenciacion
      //std::cout<< "result: " << result;
    }
    base*=base;//prepara la base para el siguiente proceso
    exp>>=1;//el exponente se divide entre dos
    //std::cout << std::endl;
  }
  return result;
}

bool MillerRabin(num d, num n){
  num x;
  const num base = num(2);
  x = power(base,d,n);
  if (x==1 || x==n-1)
    return true;
  while (d != n-1){
    x = (x * x) % n;
    d*=2;
    if (x == 1)
      return false;
    if (x == n-1)
      return true;
  }
  return false;
}

bool isPrime(num prime,int k){
  if ((prime&1)==0 && prime!=2)
    return false;
  num n = prime-1;
  while ((n&1)==0){
    n/=2;
  }
  for (int i=0; i<k; ++i){
    if (MillerRabin(n,prime)==false)
      return false;
  }
  return true;
}

void setSemilla(num &semilla, int MINBITS){
	int bitState=(int)clock();
	int i=0;	
	if (bitState&1)
		i=1;
	for (;i<MINBITS;i+=2){
		bitState=(int)clock();
		if (bitState&1)
			SwitchBit(semilla,i);	
	}
}

num ranPrimeKey(int MINBITS){
	num randomNumber;
	int len=MINBITS/8;
  std::ifstream generater;
  generater.open("dev/urandom", std::ios_base::in);
  char bytes[len];
  generater.read(bytes,len);
  generater.close();
  ZZFromBytes(randomNumber,(unsigned char*)bytes,len);
  setSemilla(randomNumber, MINBITS);
  if ((randomNumber&1)==0)
    ++randomNumber;
  while (!(isPrime(randomNumber,4))){
    randomNumber+=2;
  }
  return randomNumber;
}
