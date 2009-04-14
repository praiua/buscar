#ifndef paesa_h
#define paesa_h

#include <math.h>
#include "algBus.h"

class Paesa: public AlgoritmoBusqueda {
public:

  Paesa(){
    if( !fRegistrado ) {
      AlgoritmoBusqueda::registrar(this);
      fRegistrado = true;
    }
  };

  Paesa(const char data[]);

  AlgoritmoBusqueda *crear( char data[]) {
    return new Paesa(data);
  }

  void preprocesar(); 

  virtual
  void buscar( double &disMin, int &masCercano);

  void escribirCabeceras() {
    AlgoritmoBusqueda::escribirCabeceras();
    printf( "%5s", "upiv");
    printf( "%5s", "pmc");
    printf( "%4s", "rep");
  }

  void escribirCaracteristicas() {
    AlgoritmoBusqueda::escribirCaracteristicas();
    printf( "%5.1f", num_used_pivots/double(ora->numMues()));
    printf( "%5.1f", gpaso_mas_cercano/double(ora->numMues()));
    printf( "%4d", gRep);
  }

  const char *nombre() {
    return "paesa";
  }

  const char *info() {
    return "\
paesa ''			AESA\n";
  }

private:

  static bool fRegistrado;

  int np;              // número de prototipos
  int *pivot;          // Pivotes
  double **tdist;      // tabla de distancias
  int num_used_pivots;
  int gpaso_mas_cercano;
  int gRep;

};

#endif
