#include <iostream>
#include <string>
#include "random_primes.h"
//#define BITS 512

num modulo(num a, num b){  
 	num r=a-((a/b)*b);
  if (r<0) 
		r+=b;
  return r;
}

num raise(num base, num exp){
  num number = num(1);
  while (exp>0){
    number*=base;
    --exp;
  }
  return number;
}

num inversa(num a, num b){
  num r,r1=a,r2=b,s,s1= num(1),s2=num(0),q;
  while (r2>0){
    q=r1/r2;

    r=r1-(q*r2);
    r1=r2;
    r2=r;

    s=s1-(q*s2);
    s1=s2;
    s2=s;
  }
  return s1;
}


num RootPrimitiveGeneration(num p){
	num i;
	num q;
	q = (p-1)>>1;
	for (i=1;i<p-1;++i){
		if (power(i,(num)2,p)!=1 && power(i,q,p)!=1)
			return i;
	}
	return (num) -1;
}

void ElGamal_KeyGeneration(num &e1, num &e2, num &p, num &d, int BITS){
	p = ranPrimeKey(BITS);
	e1 = RootPrimitiveGeneration(p);
	while (d<=2 || d>=p-2){
		std::cout << "Escoger desde 2 hasta " << p << std::endl;
		std::cin >> d;
	}
	e2 = power(e1, d, p);
}

void ElGamal_Encryption(num &C1, num &C2, num e1, num e2, num p, std::string &word){
	std::string abecedario="abcdefghijklmnopqrstuvwxyz";
	num len= num((long)abecedario.size());
	num r=num(0);
	while (r<=2 || r>=p-2){
    std::cout << "Escoger desde 2 hasta " << p << std::endl;
    std::cin >> r;
  }
	C1 = power(e1,r,p);
	num Km;
	Km = power(e2,r,p);
	num numberBlock=num(0);
	num numberWord=num(0);
	num j=num(0);
	for(long i=0; i<word.size(); ++i,++j){
		numberBlock=abecedario.find(word[word.size()-i-1]);
		numberWord+=numberBlock*raise(len,j);
	}
	std::cout<<"NumberWord: "<<numberWord<<std::endl;
	word="";
	std::cout << "P: " << numberWord  << std::endl;
	C2 = modulo(numberWord*Km,p);
	num quotient=C2;
	int result=0;
	while(quotient>=len){
    result = conv<int>(modulo(quotient,len));
    quotient/=len;
    word=abecedario[result]+word;
  }
	result = conv<int>(modulo(quotient,len));
  word=abecedario[result]+word;

}
void ElGamal_Decryption(num d, num p, num &C1, num &C2, std::string &word){
	std::string abecedario="abcdefghijklmnopqrstuvwxyz";	
	num len=num((long)abecedario.size());
	C1 = modulo(inversa(power(C1,d,p),p),p);		
	C2 = modulo(C1*C2,p);
	num quotient=num(C2);
	word = "";
	int result=0;
	while(quotient>=len){
		result = conv<int>(modulo(quotient,len));
		quotient /= len;
		word=abecedario[result]+word;
	}
  result = conv<int>(modulo(quotient,len));	
	word=abecedario[result]+word;

}
main(){
	num e1,e2,p,d;
	std::cout<<"Numero de bits, potencia de 2: ";
	int BITS;
	std::cin>>BITS;
	ElGamal_KeyGeneration(e1,e2,p,d,BITS);
	std::cout << "e1: " << e1 << std::endl;
	std::cout << "e2: " << e2 << std::endl;
	std::cout << "p: " << p << std::endl;
	std::cout << "d: " << d << std::endl;
	std::cout << "------------------------"<< std::endl;
	num C1,C2;
	std::string word;
  std::cout<<"Palabra: ";
  std::cin>>word;
	ElGamal_Encryption(C1,C2,e1,e2,p, word);
	std::cout << "C1: " << C1 << std::endl;
  std::cout << "C2: " << C2 << std::endl;
  std::cout << "word: " << word << std::endl;
	std::cout << "------------------------"<< std::endl;
	ElGamal_Decryption(d,p,C1,C2,word);
	std::cout << "C1: " << C1 << std::endl;
	std::cout << "C2: " << C2 << std::endl;
  std::cout << "word: " << word << std::endl;
	return 0;
}
