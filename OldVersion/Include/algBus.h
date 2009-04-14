//---------------------------------------------------------------
//
// Clase Base del algoritmo de búsqueda
//

#ifndef algBus_h
#define algBus_h

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "oraculo.h"


class AlgoritmoBusqueda {
public:

//  typedef AlgoritmoBusqueda* (AlgoritmoBusqueda:: *TCrear)();

  virtual
  void fijarOraculo( Oraculo *o ){
    ora = o;
  };

  Oraculo* oraculo() {
    return ora;
  };

  virtual
  void preprocesar() {
    nBusquedas = 0;
  };

  virtual
  void buscar(double &, int &) {
    nBusquedas++;
  };

  virtual
  void escribirCabeceras(){
    ora->escribirCabeceras();
    printf("%8s ","alg");
  };

  virtual
  void escribirCaracteristicas(){
    ora->escribirCaracteristicas();
    printf("%8s ",nombre());
  };

  virtual
  AlgoritmoBusqueda* crear( char []){
    cerr << "Error: este método nunca deberia invocarse" << endl;
    exit(-1);
  };

  static
  AlgoritmoBusqueda *crear(char nombre[], char data[]) {
    for( int i = 0; i < nClases; i++ )
      if( strcmp(nombre, fClase[i]->nombre()) == 0 )
	  return fClase[i]->crear(data);

    return 0;
  }

  static
  void registrar( AlgoritmoBusqueda *ab) {
    fClase[nClases] = ab;
    nClases++;
    if( nClases == kMaxClases ) {
      cerr << "Error: el espacio de nombres para los algoritmos está lleno" 
	   << endl;
      exit(-1);
    }
  }

  static
  void escribirInfo() {
    for( int i = 0; i < nClases; i++ )
      cerr << fClase[i]->info() << endl;
  }

  virtual
  const char* nombre() = 0;

  virtual
  const char* info() = 0;

  virtual ~AlgoritmoBusqueda(){};
  

private:

  static const int kMaxClases = 100; 
  static int nClases;
  static AlgoritmoBusqueda* fClase[kMaxClases];

protected:

  int nBusquedas;
  Oraculo *ora;
};

#endif
