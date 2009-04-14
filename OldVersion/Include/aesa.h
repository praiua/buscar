#ifndef aesa_h
#define aesa_h

#include <math.h>
#include "algBus.h"

class Aesa: public AlgoritmoBusqueda {
public:

  Aesa(){
    if( !fRegistrado ) {
      AlgoritmoBusqueda::registrar(this);
      fRegistrado = true;
    }
  };

  Aesa(const char data[]);

  AlgoritmoBusqueda *crear( char data[]) {
    return new Aesa(data);
  }

  void preprocesar(); 

  virtual
  void buscar( double &disMin, int &masCercano);

  void escribirCabeceras() {
    AlgoritmoBusqueda::escribirCabeceras();
    printf( "%5s", "pmc");
  }

  void escribirCaracteristicas() {
    AlgoritmoBusqueda::escribirCaracteristicas();
    printf( "%5.1f", gpaso_mas_cercano/double(ora->numMues()));
  }

  const char *nombre() {
    return "aesa";
  }

  const char *info() {
    return "\
aesa ''			AESA\n";
  }

private:

  static bool fRegistrado;

  int np;              // número de prototipos
  double **tdist;      // tabla de distancias
  int gpaso_mas_cercano;

};

#endif
