#ifndef treeOrd_h
#define treeOrd_h

#include "algBus.h"

class TreeTab: public AlgoritmoBusqueda {
public:

  TreeTab(){
    if( !fRegistrado ) {
      AlgoritmoBusqueda::registrar(this);
      fRegistrado = true;
      return;
    }
    cout << "ERROR en TreeTab(): esto no debería suceder" << endl;
    exit(-1);
  };

  TreeTab(char data[]);

  AlgoritmoBusqueda *crear( char data[]) {
    return new TreeTab(data);
  }

  void preprocesar() {
    AlgoritmoBusqueda::preprocesar();

    if( raiz != 0 )
      delete raiz;
    raiz = construirArbol();
    if( tabDis != 0 )
      delete[] tabDis;
    if( tabProt != 0 )
      delete[] tabProt;
    construirTabla();
    nodosVisitados = 0;
  }

  virtual
  void buscar( double &disMin, int &masCercano) {
    buscarArbol( raiz, disMin, masCercano);
    buscarTabla( disMin, masCercano); 
  }

  void escribirCabeceras() {
    AlgoritmoBusqueda::escribirCabeceras();
    printf(" %7s %5s", "NodVis", "prof");
  }

  void escribirCaracteristicas() {
    AlgoritmoBusqueda::escribirCaracteristicas();
    printf(" %7d %5d", 
	   nodosVisitados/ora->numMues(), profundidad(raiz));
  }

  const char *nombre() {
    return "treetab";
  }

  const char *info() {
    return "\
treetab ''              Usa un arbol para encontrar un vecino cercano\n\
			a continuacion busca en una tabla el mas cercano\n\
			de verdad\n";
  }

// esto deberia ser private
//
  class Arbol {
  public:
    Arbol(): hijo_izq(0), hijo_der(0){};

    ~Arbol() {
      if( hijo_izq != 0)
	delete hijo_izq;
      if( hijo_der != 0)
	delete hijo_der;
    }

    friend int profundidad( TreeTab::Arbol* a);

    Arbol *hijo_izq;
    Arbol *hijo_der;
    int rep;
  };

private:

  Arbol *construirArbol( int p[], const int numP, const int repI);
  Arbol* construirArbol();

  void construirTabla();

  void buscarArbol( const Arbol  *a, 
	                  double &disMin,
	                  int    &masCecano);

  void buscarArbol( const Arbol  *a, 
	            const double dis_izq,
	                  double &disMin,
	                  int    &masCecano);
  Arbol *raiz;

  void buscarTabla( double &disMin, int &masCercano); 

  int    np;
  double *tabDis;
  int    *tabProt;

  int nodosVisitados;
  static bool fRegistrado;
};

#endif
