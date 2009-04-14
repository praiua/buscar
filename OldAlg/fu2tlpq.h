#ifndef fu2tlpq_h
#define fu2tlpq_h

// fu2tl. h,v 1.3 2006/06/20 13:00:46 thollard Exp 

#include "algBus.h"
#include "baseTable.h"
#include "staticTable.h"

class Fu2tlpq: public AlgoritmoBusqueda {
public:

  Fu2tlpq(){
    if( !fRegistrado ) {
      AlgoritmoBusqueda::registrar(this);
      fRegistrado = true;
      return;
    }
    cerr << "ERROR en Fu2tlpq(): esto no debería suceder" << endl;
    exit(-1);
  };

  Fu2tlpq(char data[]);

  AlgoritmoBusqueda *crear( char data[]) {
    return new Fu2tlpq(data);
  }
  
  void preprocesar() {
    AlgoritmoBusqueda::preprocesar();
    if( raiz != 0 )
      delete raiz;
    raiz = construirArbol();
    np=ora->numProt();
    if( regla_t) {
      if (table_type == 's') {
	if (tabla) delete tabla;
	tabla = new StaticTable(np,contNodos);
      }
      else {cerr << "table h and p not yet implemented; \n Sorry "; exit(-1);}      
      rellenarTabla();
      if( dnnf >= 0.0 || dnnt >= 0.0 )	
	podarTabla();      
    }
    nodosVisitados = 0;
  }

  virtual
  void buscar( double &dis_min, int &mas_cercano);

  void escribirCabeceras() {
    AlgoritmoBusqueda::escribirCabeceras();
      printf(" %7s %5s %5s", "NodVis", "prof", "reg");
    if ( regla_t) printf(" %10s", "util_t");
    if( dnnf >= 0.0 )
      printf(" %8s", "dnnf");
    if( dnnt >= 0.0 )
      printf(" %8s", "dnnt");
    if( dnnf >= 0.0 || dnnt >= 0.0 )  
      printf(" %5s", "podas");
  }

  void escribirCaracteristicas() {
    AlgoritmoBusqueda::escribirCaracteristicas();
    char reg[5];
    reg[0] = '\0';
    if( regla_f ) strcat(reg,"f");
    if( regla_s ) strcat(reg,"s");
    if( regla_u ) strcat(reg,"u");
    if( regla_g ) strcat(reg,"g");
    if( regla_t ) strcat(reg,"t");
    printf(" %7d %5d %5s", 
	   nodosVisitados/ora->numMues(), profundidad(raiz), reg);
    if( dnnf>= 0.0 )
      printf(" %8g ", dnnf);
    if( dnnt >= 0.0 )
      printf(" %8g ", dnnt);
    if (dnnf >= 0.0 || dnnt >= 0.0)
      printf(" %5.1f%%", (100.0 * cpodas)/ celem);

  }

  const char *nombre() {
    return "fu2tlpq";
  }

  const char* info();

// esto deberia ser private
//
  class Arbol {
  public:
    Arbol(): hijo_izq(0), hijo_der(0), dis_su_rep(0), dis_otro_rep(0) {};

    ~Arbol() {
      if( hijo_izq != 0)
	delete hijo_izq;
      if( hijo_der != 0)
	delete hijo_der;
      if( dis_su_rep != 0 )
	delete dis_su_rep;
      if( dis_otro_rep != 0 )
	delete dis_otro_rep;
    }

    friend int profundidad( Fu2tlpq::Arbol* a);


    Arbol *hijo_izq;
    Arbol *hijo_der;
    int rep;
    double radio_ext; // radius 
    double radio_int; // distacia al mas cercano del hermano

    int num_bordes;
    double *dis_su_rep;
    double *dis_otro_rep;

    int numNodo;
  };

private:

  Arbol *construirArbol( int p[], const int numP, const int repI);
  Arbol* construirArbol();

  void buscar( const Arbol *a, 
               const double dis_izq, 
	             double &disMin,
	             int    &masCecano);

  void buscar( const Arbol *actual, 
               const double dis_act, 
               const Arbol *sibling,
               const double dis_sib,
	             double &disMin,
	             int    &masCecano);

  //  void rellenarLinea( int p, double d[], const Arbol *a );
  void rellenarLinea(const int, const Arbol *);
  void rellenarTabla();


  void podarLinea(const int, const Arbol *);
  void podarTabla();

  Arbol *raiz;
  int contNodos;

  Table *tabla; //  generic table for storing distancia de prototipo a nodo
  
  int numProtBase;
  int contProto;
  int np;
  bool regla_f;
  bool regla_s;
  bool regla_u;
  bool regla_g;
  bool regla_t;
  char table_type; // s for static, h for hash table base, p for pruned
  double dnnf, dnnt;
  int celem;
  int cpodas;
  int *ProtoBase;
  int nodosVisitados;
  static bool fRegistrado;
  
};

#endif
