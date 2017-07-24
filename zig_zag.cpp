#include <iostream>
#include <string>

using namespace std;

class Zig_zag{
private:
  string msje_cifrado;
  string texto_plano;
  int clave;
public:
  Zig_zag(string _texto_plano): texto_plano(_texto_plano) {}
  Zig_zag(string _msje_cifrado, int _clave):msje_cifrado(_msje_cifrado), clave(_clave) {}
  string cifrado_zig_zag(string word, int row){
    string new_word;
    this->clave=row;
    int i,k,j;
    for (i=0;i<this->clave;++i){
      j=i;
      for (k=1;j<word.size();++k){
        new_word+=word[j];
        if (this->clave==1)
          ++j;
        else if (i==0 || i==this->clave-1){
          j+=(this->clave-1)*2;
        }
        else if (k%2==0){
          j+=i*2;
        }
        else
          j+=(this->clave-1-i)*2;
      }
    }
    this->msje_cifrado=new_word;
    return new_word;
  }
  string descifrado_zig_zag(string word, int row){
    string new_word=word;
    int i,k,j,cont=0;
    for (i=0;i<row;++i){
      j=i;
      for (k=1;j<word.size();++k){
        new_word[j]=word[cont];
        if (row==1)
          ++j;
        else if (i==0 || i==row-1){
          j+=(row-1)*2;
        }
        else if (k%2==0){
          j+=i*2;
        }
        else
          j+=(row-1-i)*2;
        ++cont;
      }
    }
    this->texto_plano=new_word;
    return new_word;
  }
  string get_texto_plano() {return this->texto_plano;}
  string get_msje_cifrado() {return this->msje_cifrado;}
};


main(){
  string word;
	cout<< "Texto para cifrar: ";
	cin>>word;
  cout<< "Clave: ";
	int clave;
  cin >> clave;
  Zig_zag emisor(word);
  cout << emisor.cifrado_zig_zag(emisor.get_texto_plano(),clave) << endl;
  Zig_zag receptor(emisor.get_msje_cifrado(),clave);
  cout << receptor.descifrado_zig_zag(receptor.get_msje_cifrado(),clave) << endl;
  return 0;
}
