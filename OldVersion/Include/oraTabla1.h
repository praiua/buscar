#ifndef oraTabla1_h
#define oraTabla1_h

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include "oraculo.h"

class OraTabla1 : public Oraculo {
public:

  OraTabla1() {
    if( !fRegistrado ) {
      Oraculo::registrar(this);
      fRegistrado = true;
      return;
    }
    cerr << "ERROR en OraTabla1(): esto no debería pasar" << endl;
    exit(-1);
  }

  OraTabla1( char data[] );

  Oraculo *crear( char data[] ) {
    return new OraTabla1(data);
  }

  virtual
  double distancia( const int p1, const int p2 ) {
    return tabla[prot[p1] * numPuntos + prot[p2] ];
  }

  virtual
  double distancia( const int p) {
    Oraculo::distancia(p);
    return tabla[prot[p] * numPuntos + mues[muesAct] ];
  }

  virtual
  void preprocesar();

  virtual
  void escribirCabeceras(){
    Oraculo::escribirCabeceras();
    printf(" %12s", "fich");
  };

  virtual 
  void escribirCaracteristicas(){
    Oraculo::escribirCaracteristicas();
    printf(" %12s", basename(fNomFich));
  };

  const char *nombre() {
    return "tab1";
  }

  const char *info(){
    return "\
tab1 '<nom_fich>'  Obtiene la tabla de distancias del fichero\n";
  }

  ~OraTabla1() {
    if( tabla != 0 ) 
      delete tabla;
  }

private:

  static bool fRegistrado;

  char   fNomFich[100];
  int    numPuntos;
  double *tabla;

  int *prot;
  int *mues;

};

#endif
