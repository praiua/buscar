#ifndef fukunaga_h
#define fukunaga_h

#include <stdio.h>
#include "algBus.h"
#include "oraculo.h"

class Fukunaga: public AlgoritmoBusqueda {
public:

  Fukunaga() : raiz(0) {
    if( !fRegistrado ) {
      AlgoritmoBusqueda::registrar(this);
      fRegistrado = true;
    }
  };
   
  Fukunaga( char data[] );

  AlgoritmoBusqueda *crear(char data[]) {
    return new Fukunaga(data);
  }

  void preprocesar();

  void buscar(double &disMin, int &masCercano);

  void escribirCabeceras() {
    AlgoritmoBusqueda::escribirCabeceras();
    printf(" %3s %7s %7s %5s", "nh", "rep", "NodVis", "prof");
  }

  void escribirCaracteristicas() {
    AlgoritmoBusqueda::escribirCaracteristicas();
    printf(" %3d %7s %7d %5d", num_hijos, alg_rep, 
	                       nodosVisitados/ora->numMues(), 
			       profundidad(raiz));

  }

  const char *nombre() {
    return "fuku";
  }

  const char *info() {
    return "\
fuku '[opciones]'       Algoritmo de Fukunaga.\n\
  -nh <nh>              Número de hijos.\n\
  -ar algoritmo         Algoritmo de reparto (maxsep, isodata, cmeans).\n\
  -r  [12]              Regla de eliminación\n";
  }

  ~Fukunaga() {
    if( raiz != 0 )
      delete raiz;
  }

private:

  class Arbol {
  public:
    Arbol() {};

    ~Arbol() {
      for( int i = 0; i < num_hijos; i++ ) {
	delete hijo[i];
	delete radio_int[i];
      }
      delete rep;
      delete hijo;
      delete radio_ext;
      delete radio_int;
    }

    friend int profundidad(Fukunaga::Arbol* a);

    int num_hijos;
    int *rep;
    Arbol **hijo;
    double *radio_ext;
    double **radio_int;
  };

  
  Fukunaga::Arbol *construirArbol(int p[], const int num_p);
  void buscar( const Fukunaga::Arbol *a, double &disMin, int &masCercano);

  void (*cluster) ( const int c,
                          Oraculo *o, 
		    const int prot[], 
		    const int numP,
		          int clase[],
		          int centroide[]);

  void iniciar( int nh, char *aRep);

  static bool fRegistrado;

  int num_hijos;
  char alg_rep[10];
  Fukunaga::Arbol *raiz;
  int nodosVisitados;
  bool regla1;
  bool regla2;

  friend int profundidad(Fukunaga::Arbol* a);
};

#endif 
