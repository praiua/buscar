#ifndef fu2_h
#define fu2_h

#include "algBus.h"

class Fu2: public AlgoritmoBusqueda {
public:

  Fu2(){
    if( !fRegistrado ) {
      AlgoritmoBusqueda::registrar(this);
      fRegistrado = true;
      return;
    }
    cout << "ERROR en Fu2(): esto no debería suceder" << endl;
    exit(-1);
  };

  Fu2(char data[]);

  AlgoritmoBusqueda *crear( char data[]) {
    return new Fu2(data);
  }

  void preprocesar() {
    AlgoritmoBusqueda::preprocesar();

    if( raiz != 0 )
      delete raiz;
    raiz = construirArbol();
    nodosVisitados = 0;
  }

  virtual
  void buscar( double &dis_min, int &mas_cercano) {
    dis_min = MAXDOUBLE;
    mas_cercano = -1;
    buscar(raiz, dis_min, mas_cercano);
  }

  void escribirCabeceras() {
    AlgoritmoBusqueda::escribirCabeceras();
    printf(" %7s %5s %3s", "NodVis", "prof", "reg");
  }

  void escribirCaracteristicas() {
    AlgoritmoBusqueda::escribirCaracteristicas();
    char reg[4];
    reg[0] = '\0';
    if( regla1 ) strcat(reg,"1");
    if( regla2 ) strcat(reg,"2");
    if( regla3 ) strcat(reg,"3");

    printf(" %7d %5d %3s", 
	   nodosVisitados/ora->numMues(), profundidad(raiz), reg);
  }

  const char *nombre() {
    return "fu2";
  }

  const char *info() {
    return "\
fu2 '[opciones]'        Caso especial del algoritmo de Fukunaga con\n\
                        solo dos hijos.\n\
  -r [123]              Reglas de eliminación\n\
  -rep                  Genera un árbol equilibrado repartiendo los\n\
                        prototipos según su representante mas cercano\n";
  }

private:

  class Arbol {
  public:
    Arbol(): hijo_izq(0), hijo_der(0) {};

    ~Arbol() {
      if( hijo_izq != 0)
	delete hijo_izq;
      if( hijo_der != 0)
	delete hijo_der;
    }

    int rep_izq;
    double radio_izq;
    double radio_int_izq;
    Arbol *hijo_izq;

    int rep_der;
    double radio_der;
    double radio_int_der;
    Arbol *hijo_der;

    friend int profundidad( Fu2::Arbol* a);

  };


  Arbol *construirArbol( int p[], const int numP);
  Arbol* construirArbol();

  void buscar( const Arbol *a, 
	             double &disMin,
	             int    &masCecano);

  Arbol *raiz;
  bool regla1;
  bool regla2;
  bool regla3;
  bool equilibrado;
  bool repartiendo;

  int nodosVisitados;
  static bool fRegistrado;

  friend int profundidad( Fu2::Arbol* a);
};

#endif
