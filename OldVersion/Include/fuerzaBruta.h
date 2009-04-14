#ifndef fuerzaBruta_h
#define fuerzaBruta_h

#include <math.h>
#include "algBus.h"

class FuerzaBruta: public AlgoritmoBusqueda {
public:

  FuerzaBruta(){
    if( !fRegistrado ) {
      AlgoritmoBusqueda::registrar(this);
      fRegistrado = true;
    }
  };

  FuerzaBruta(const char data[]){
    if( strcmp(data,"") != 0 )
      cerr << "ERROR: datos no válidos para este algoritmo" << endl;
  };

  AlgoritmoBusqueda *crear( char data[]) {
    return new FuerzaBruta(data);
  }

  void preprocesar() {
    AlgoritmoBusqueda::preprocesar();
    suma_distancias = 0.0;
  }

  virtual
  void buscar( double &disMin, int &masCercano) {
    masCercano = 0;
    disMin = ora->distancia(0);
    for( int i = 1; i < ora->numProt(); i++) {
      double dis = ora->distancia(i);
      if( dis < disMin ) {
        disMin = dis;
        masCercano = i;
      }
    }
    suma_distancias += disMin;
  }

  void escribirCabeceras() {
    AlgoritmoBusqueda::escribirCabeceras();
    printf(" %7s","DisMed");
  }

  void escribirCaracteristicas() {
    AlgoritmoBusqueda::escribirCaracteristicas();
    printf(" %7.4f", suma_distancias/ora->numMues());
  }

  const char *nombre() {
    return "fb";
  }

  const char *info() {
    return "\
fb ''                   Fuerza bruta\n";
  }

private:

  static bool fRegistrado;
  double suma_distancias;
};

#endif
