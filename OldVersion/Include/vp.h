#ifndef fu2_h
#define fu2_h

#include "algBus.h"

class Vp: public AlgoritmoBusqueda {
public:

  Vp(){
    if( !fRegistrado ) {
      AlgoritmoBusqueda::registrar(this);
      fRegistrado = true;
      return;
    }
    cout << "ERROR en Vp(): esto no debería suceder" << endl;
    exit(-1);
  };

  Vp(char data[]);

  AlgoritmoBusqueda *crear( char data[]) {
    return new Vp(data);
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
    nivel = 0;
//    cout << "--------------------------" << endl;
    buscar(raiz, dis_min, mas_cercano);
  }

  void escribirCabeceras() {
    AlgoritmoBusqueda::escribirCabeceras();
    printf(" %3s %7s %5s %3s", "nh", "NodVis", "prof", "reg");
  }

  void escribirCaracteristicas() {
    AlgoritmoBusqueda::escribirCaracteristicas();
    char reg[4];
    reg[0] = '\0';
    if( regla1 ) strcat(reg,"1");
    if( regla2 ) strcat(reg,"2");

    printf(" %3d %7d %5d %3s", 
	   num_hijos, nodosVisitados/ora->numMues(), profundidad(raiz), reg);
  }

  const char *nombre() {
    return "vp";
  }

  const char *info() {
    return "\
vp '[opciones]'         Organiza los grupos según su distancia a un\n\
                        prototipo.\n\
  -nh <nh>              Número de hijos\n\
  -r [12]               Reglas de eliminación a utilizar\n\
  -ext                  Coje los representantes en los extremos\n";
  }

// esto deberia ser private

  class Arbol {
  public:
    Arbol() {};

    ~Arbol() {
      for( int i = 0; i < num_hijos; i++ ) {
	delete hijo[i];
      }
      delete radio_int;
      delete radio_ext;
      delete hijo;
    }

    friend int profundidad( Vp::Arbol* a);
    friend void comprobar( Vp::Arbol *a, int num_punt);
    friend void comprobar( const Vp::Arbol *a, bool *usado, int num_punt);


    int rep;
    int num_hijos;
    double *radio_int;
    double *radio_ext;
    Arbol **hijo;
  };

private:

  Arbol *construirArbol( int p[], const int numP);
  Arbol* construirArbol();

  void buscar( const Arbol *a, 
	             double &disMin,
	             int    &masCecano);

  Arbol *raiz;
  bool regla1;
  bool regla2;
  bool extremando;
  int  num_hijos;
  int  nivel;

  int nodosVisitados;
  static bool fRegistrado;
};

#endif
