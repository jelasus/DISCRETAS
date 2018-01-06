#include<NTL/ZZ.h>
#include<string>
#include<sstream>
#include<fstream>
#include<sstream>
#include<ctime>
#include"binarygcd.h"
using namespace NTL;
using namespace std;
typedef ZZ num;
class RSA{
	public:
		RSA(unsigned int bits);
		RSA(num primo1, num primo2);
		//virtual ~RSAC(void);
		num exponenciacion(num base, num exponente, num modulo);
		num resto_chino(num base);
		std::string cifrar(std::string texto_plano);
		std::string descifra_mensaje(std::string texto_cifrado);
		num euclides(num numero1, num numero2);
		//vector
		num inversa(num numero, num modulo);
		num generar_aleatorio(unsigned int bits);
		bool test_primalidad(num numero);
		static string alfabeto;
		int digitos(num number);
	private:
		void clave(num &e, unsigned int _bits);
		bool MillerRabin(num n, num primo);
		bool esPrimo(num primo, int k);
		void establecerSemilla(num &semilla, unsigned int _bits);
		num mcd(num numero, num modulo);
		num exp(num base, int exponente);
		num convertToNum(std::string number);
		bool fermatTheorem(num numero, int k);
		num p;
		num q;
		num e;
		num N;
		num d;		
};

