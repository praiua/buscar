#ifndef oraculo_h
#define oraculo_h

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Oraculo {
public:

  Oraculo() : nProt(0), nMues(0), sem(0) {};
  
  virtual
  double distancia( const int p1, const int p2 ) = 0 ;

  virtual
  double distancia( const int) {
    contDis++;
    return 0;
  };

  int numProt() const {
    return nProt;
  }
    
  int numMues() const {
    return nMues;
  }
    
  void fijarNumProt( const int i) {
    nProt = i;
  }
    
  void fijarNumMues( const int i ){
    nMues = i;
  }
    
  virtual
  void semilla(const int s) {
    sem = s;
    srand48(s);
  };

  virtual 
  void preprocesar() {
    contDis = 0;
    muesAct = 0;
  };

  bool nuevaMuestra() {
    if( muesAct < nMues )
      muesAct++;
    return muesAct < nMues;
  }

  virtual
  void escribirCabeceras(){
    printf(" %5s %6s %6s %5s %8s", "ora", "nMues", "nProt", "sem", "dist");
  };

  virtual 
  void escribirCaracteristicas(){
    printf(" %5s %6d %6d %5d %8.2f", 
	nombre(), nMues, nProt, sem, contDis/double(muesAct));
  }

  virtual
  Oraculo* crear( char[]){
    cerr << "Error en Oraculo: este método nunca deberia invocarse" << endl;
    exit(-1);
  }

  static
  Oraculo *crear( char nombre[], char data[]) {
    for( int i = 0; i < nClase; i++ )
      if( strcmp(nombre, fClase[i]->nombre()) == 0)
	return fClase[i]->crear(data);
    return 0;
  }
  
  static
  void registrar( Oraculo *o) {
    fClase[nClase] = o;
    nClase++;
    if( nClase == kMaxClases ) {
      cerr << "Error: el espacio de nombres para oraculos está lleno" << endl;
      exit(-1);
    }
  }

  static
  void escribirInfo() {
    for( int i = 0; i < nClase; i++ )
      cerr << fClase[i]->info() << endl;
  }

  virtual
  const char *nombre() = 0;

  virtual
  const char *info() = 0;

  virtual
  ~Oraculo(){};
    
private:

  static const int kMaxClases = 100;
  static int nClase;
  static Oraculo* fClase[kMaxClases];

  int contDis;

protected:

  int muesAct;
  int nProt;
  int nMues;
  int sem;
};

#endif
