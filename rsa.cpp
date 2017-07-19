#include<iostream>
#include"rsa.h"
std::string RSA::alfabeto = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
RSA::RSA(int _bits){
	this->p=generar_aleatorio(_bits/2);
	this->q=generar_aleatorio(_bits/2);
	std::cout << this->p << std::endl;
	std::cout << this->q << std::endl;
	this->N=this->q*this->p;
	std::cout<<"Ingrese clave: ";
	std::cin>>this->e;
	clave(this->e);
}
bool RSA::fermatTheorem(num n, int k){
	if (n <= 1 || n == 4)  return false;
   if (n <= 3) return true;
   while (k>0){
       num a =num(conv<num>(2 + rand())%(n-4));  
       if (exponenciacion(a, n-1, n) != 1)
          return false;
       k--;
    }
    return true;
}
num RSA::mcd(num numero, num modulo){
	return binaryGCD(numero,modulo);
}
void RSA::clave(num &_e){
	num fi=num((this->p-1)*(this->q-1));	
	while(mcd(_e,fi)!=1){
		std::cout<<"Ingrese otra clave: "; 
		std::cin>>_e;
	}
}
num RSA::generar_aleatorio(int _bits){
	num randomNumber;
  int len=_bits/8;
  std::ifstream generater;
  generater.open("dev/urandom", std::ios_base::in);
  char bytes[len];
  generater.read(bytes,len);
  generater.close();
  ZZFromBytes(randomNumber,(unsigned char*)bytes,len);
  establecerSemilla(randomNumber, _bits);
  if ((randomNumber&1)==0)
    ++randomNumber;
  while (!(fermatTheorem(randomNumber,10000))){
    randomNumber+=2;
  }
  return randomNumber;
}
RSA::RSA(num primo1, num primo2): p(primo1), q(primo2), N(this->p*this->q) {}
num RSA::exponenciacion(num base, num exponente, num modulo){
	num result;//resultado final, empieza en uno porque el primer resultado es igual a la base%modulo
	result=1;
  while(exponente>0){
    base=base%modulo; //el numero que se enviara a multiplicar en la siguiente operacion de exponenciacion
    if ((exponente & 1) == 1){//si el exponente es impar..
      result=(base*result)%modulo;//se hace el resultado de la correspondiente exponenciacion
      //std::cout<< "result: " << result;
    }
    base*=base;//prepara la base para el siguiente proceso
    exponente>>=1;//el exponente se divide entre dos
    //std::cout << std::endl;
  }
  return result;
}
bool RSA::esPrimo(num prime, int k){
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
void RSA::establecerSemilla(num &semilla, int _bits){
	int bitState=(int)clock();
  int i=0;
  if (bitState&1)
    i=1;
  for (;i<_bits;i+=2){
    bitState=(int)clock();
    if (bitState&1)
      SwitchBit(semilla,i);
  }
}

bool RSA::MillerRabin(num n, num primo){
	num x;
  num base = num(conv<num>(2+rand())%(n-4));
  x = exponenciacion(base,d,n);
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

bool RSA::test_primalidad(num numero){
	int k;
	k=100;
	return fermatTheorem(numero,k);
}

std::string RSA::cifrar(std::string texto_plano){
	int bloque=digitos(this->N)-1;
	std::string bloqueString;
	std::stringstream ss;
	int val;
	int i;
	for (i=0; i<texto_plano.size();++i){
		val=RSA::alfabeto.find(texto_plano[i]);
		ss << val;
		if (val<10)
			bloqueString+="0";
		bloqueString+=ss.str();
		ss.str("");
	}
	const std::string nulo="26";
	std::cout << "N: "<< this->N << std::endl;
	std::cout << "Bloque: "<< bloque << std::endl;
  std::cout << "TAMAÑO: "<< bloqueString.size() << std::endl;
	while(bloqueString.size()%bloque!=0){
		bloqueString+=nulo;
	}
	std::cout << "BloqueString: "<< bloqueString << std::endl;
	std::cout << "Hasta aca" << std::endl;
	std::string bloqueNumero;
	texto_plano="";
	num numero;
	for(i=0;i<bloqueString.size();i+=bloque){
		numero=convertToNum(bloqueString.substr(i,bloque));
		numero=exponenciacion(numero,this->e,this->N);
		ss << numero;
		bloqueNumero=ss.str();
		ss.str("");
		while(bloqueNumero.size()<bloque+1)
			bloqueNumero="0"+bloqueNumero;
		texto_plano+=bloqueNumero;
	}
	return texto_plano;
}
std::string RSA::descifra_mensaje(std::string texto_cifrado){
	int bloque=digitos(this->N);
	int i;
	std::stringstream ss;
	num numero;
	std::string bloqueNumero;
	std::string bloqueString;
	for(i=0;i<texto_cifrado.size();i+=bloque){
    numero=convertToNum(texto_cifrado.substr(i,bloque));
    std::cout << "Pieza: "  << numero  << std::endl;
		numero=resto_chino(numero);
		std::cout << "Resultado: " << numero << std::endl;
    ss << numero;
    bloqueNumero=ss.str();
		ss.str("");
		while(bloqueNumero.size()<bloque-1)
      bloqueNumero="0"+bloqueNumero;
    bloqueString+=bloqueNumero;
  }
	std::cout<< "TEXTO PARA DESCIFRAR: "  << bloqueString << std::endl;
	int val;
	texto_cifrado="";
	for (i=0;i<bloqueString.size();i+=2){
		val=conv<int>(convertToNum(bloqueString.substr(i,2)));
		std::cout<<"VAL: " << val << std::endl;
		texto_cifrado+=RSA::alfabeto[val%RSA::alfabeto.size()];
	}
	return texto_cifrado;
}
num RSA::convertToNum(std::string cadena){
	num numero=num(0);
	int len=cadena.size();
	num diez=num(1);
	for(int i=0;i<len;++i){
		numero+=(cadena[len-i-1]-'0')*diez;
		diez*=10;
	}
	return numero;
}
num RSA::exp(num base, int exponente){
	num numero=num(1);
	while(exponente>0){
		numero*=base;
	}
	return numero;
}
int RSA::digitos(num number){
	int cont=0;
	while(number>0){
		number/=10;
		++cont;
	}
	return cont;
}
num RSA::inversa(num numero, num modulo){
  num r,r1=numero,r2=modulo,s,s1=num(1),s2=num(0),q;
  while (r2>0){
    q=r1/r2;

    r=r1-(q*r2);
    r1=r2;
    r2=r;

    s=s1-(q*s2);
    s1=s2;
    s2=s;
  }
  return s1%modulo;
}
num RSA::resto_chino(num base){
	num M1, M2, M, a1, a2;
	num fi=(this->p-1)*(this->q-1);
  this->d=inversa(this->e,fi);
	std::cout << "Inversa: " << this->d << std::endl;
	a1=exponenciacion(base,d%(this->p-1),this->p);
  a2=exponenciacion(base,d%(this->q-1),this->q);
  M = this->p*this->q;
  M1 = M/this->p;
  M2 = M/this->q;
  M=(a1*M1*inversa(M1,this->p)+a2*M2*inversa(M2,this->q))%M;
  return M;

}
main(){	
	RSA alicia(1024);
	std::string text= alicia.cifrar("DISCRETAS III");
	std::cout << text << std::endl;
	text=alicia.descifra_mensaje(text);
	std::cout << text << std::endl;	
	return 0;
}
