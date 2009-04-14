#ifndef oraDisEd_h
#define oraDisEd_h

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include "oraculo.h"
#include <string>
#include <vector>

using namespace std;

class OraDisEd : public Oraculo {
public:

  OraDisEd() {
    if( !fRegistrado ) {
      Oraculo::registrar(this);
      fRegistrado = true;
      return;
    }
    cerr << "ERROR en OraTabla1(): esto no debería pasar" << endl;
    exit(-1);
  }

  OraDisEd( char data[] );

  Oraculo *crear( char data[] ) {
    return new OraDisEd(data);
  }

  virtual
  double distancia( const int p1, const int p2 ) {
    return distancia( tabla[prot[p1]], tabla[prot[p2]]);
  }

  virtual
  double distancia( const int p) {
    Oraculo::distancia(p);
    return distancia( tabla[prot[p]], tabla[mues[muesAct]] );
  }

  string prototipo( const int p ) { // for debuging pourposes
    return tabla[prot[p]];
  }

  virtual
  void preprocesar();

  virtual
  void escribirCabeceras(){
    Oraculo::escribirCabeceras();
    if( nMuesMem == 0 )
      printf(" %12s", "fich");
  else
      printf(" %12s %12s", "f_prot", "f_mues");
  printf(" %4s %4s %4s", "Pi", "Pb", "Ps");
  };

  virtual 
  void escribirCaracteristicas(){
    Oraculo::escribirCaracteristicas();
    if( nMuesMem == 0 )
      printf(" %12s", basename(fich_prot));
  else
      printf(" %12s %12s", basename(fich_prot), basename(fich_mues));

    printf(" %4.1f %4.1f %4.1f", Pi, Pb, Ps);
  };

  const char *nombre() {
    return "edic";
  }

  const char *info(){
    return "\
edic '[opciones]'  Calcula la distancia de edición\n\
  -f <fichero>     Fichero de los prototipos\n\
  -fm <fichero>    Fichero de las muestras\n\
  -p <i> <b> <s>   Pesos de borrado, inserción y sustitución\n\
    Los ficheros de prototipos o muestras son una serie de cadenas\n\
  separadas por blancos, tabuladores, retornos de carro, etc.\n";
  }

private:

  double distancia( string c1, string c2);

  static bool fRegistrado;

  char fich_prot[100];
  char fich_mues[100];
  int  nProtMem;
  int  nMuesMem;
  vector<string> tabla;

  int *prot;
  int *mues;

  double Pi;
  double Pb;
  double Ps;
};

#endif
