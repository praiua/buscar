#ifndef oraUni_h
#define oraUni_h

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "oraculo.h"

class OraUni : public Oraculo {
public:

  OraUni() {
    if( !fRegistrado ) {
      Oraculo::registrar(this);
      fRegistrado = true;
      return;
    }
    cerr << "ERROR en OraUni(): esto no deberia pasar" << endl;
    exit(-1);
  }

  OraUni( char data[] );

  Oraculo *crear( char data[] ) {
    return new OraUni(data);
  }

  virtual
  double distancia( const int p1, const int p2 ) {
    return distancia( prot[p1], prot[p2]);
  }

  virtual
  double distancia( const int p) {
    Oraculo::distancia(p);
    return distancia( prot[p], mues[muesAct] );
  }

  virtual
  void preprocesar();

  virtual
  void escribirCabeceras(){
    Oraculo::escribirCabeceras();
    printf(" %3s", "dim");
  };

  virtual 
  void escribirCaracteristicas(){
    Oraculo::escribirCaracteristicas();
    printf(" %3d", dim);
  };

  const char *nombre() {
    return "unif";
  }

  const char *info(){
    return "\
unif '[opciones]'  Genera vectores de dimension a especificar, distribuidos\n\
                   uniformemente en el hipercubo unidad; usa la distancia\n\
		   euclidea\n\
  -d <dim>         Dimension\n\
  -i <dim>         Dimensiones independientes\n";
  }

  ~OraUni();

private:

  double distancia( double* v1, double* v2);

  static bool fRegistrado;

  int dim;
  int dimI;
  int nProtMem;
  double **prot;
  int nMuesMem;
  double **mues;

};

#endif
