#ifndef oraDisL_h
#define oraDisL_h

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include "oraculo.h"
#include <vector>

using namespace std;

class OraDisL : public Oraculo {
public:

  OraDisL() {
    if( !fRegistrado ) {
      Oraculo::registrar(this);
      fRegistrado = true;
      return;
    }
    cerr << "ERROR en OraDisL(): esto no debería pasar" << endl;
    exit(-1);
  }

  OraDisL( char data[] );

  Oraculo *crear( char data[] ) {
    return new OraDisL(data);
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

  virtual
  void preprocesar();

  virtual
  void escribirCabeceras(){
    Oraculo::escribirCabeceras();
    if( nMuesMem == 0 )
      printf(" %12s", "fich");
    else
      printf(" %12s %12s", "f_prot", "f_mues");

    printf(" %3s %5s", "dim", "n");
  
  };

  virtual 
  void escribirCaracteristicas(){
    Oraculo::escribirCaracteristicas();
    if( nMuesMem == 0 )
      printf(" %12s", basename(fich_prot));
    else
      printf(" %12s %12s", basename(fich_prot), basename(fich_mues));
    printf(" %3d %5.2lf", dim, n);

  };

  const char *nombre() {
    return "L";
  }

  const char *info(){
    return "\
L '[opciones]'  Calcula la distancia de Minkowski\n\
  -d <dimension>   Dimensión de los datos\n\
  -n <grado>       Grado de la distancia\n\
  -f <fichero>     Fichero de los prototipos\n\
  -fm <fichero>    Fichero de las muestras\n\
    Los ficheros de prototipos o muestras son una serie de reales\n\
    separados por blancos. Un vector en cada línea\n";
  }

private:

  double distancia( vector<double> v1, vector<double> c2);

  static bool fRegistrado;

  char fich_prot[100];
  char fich_mues[100];
  int  nProtMem;
  int  nMuesMem;
  int dim;
  double n;
  vector< vector<double> > tabla;

  int *prot;
  int *mues;

};

#endif
