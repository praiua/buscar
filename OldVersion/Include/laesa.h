#ifndef laesa_h 
#define laesa_h

#include <stdio.h>
#include "algBus.h"
#include "oraculo.h"

class Laesa: public AlgoritmoBusqueda {
public:

  Laesa() {
    if( !fRegistrado ) {
      AlgoritmoBusqueda::registrar(this);
      fRegistrado = true;
    }
  };
   
  Laesa( char data[] );

  AlgoritmoBusqueda *crear(char data[]) {
    return new Laesa(data);
  }

  void preprocesar();

  void buscar(double &dis_min, int &mas_cercano);

  void escribirCabeceras() {
    AlgoritmoBusqueda::escribirCabeceras();
    printf(" %3s", "pb");
  }

  void escribirCaracteristicas() {
    AlgoritmoBusqueda::escribirCaracteristicas();
    printf(" %3d", num_pb);
  }

  const char *nombre() {
    return "laesa";
  }

  const char *info() {
    return "\
laesa '[opciones]'      Algoritmo LAESA.\n\
  -pb  <pb>             Número de prototipos base.\n\
  -apb <alg>            Algoritmo de selecion de prototipos base\n\
                          (minmax, nimsup).\n";
  }

  ~Laesa() {
    
  }

private:

  static bool fRegistrado;

  int num_p;
  int num_pb;
  int *pb;
  int num_pnb;
  bool *es_base;
  double **dis_pb;
  char apb[20];
};

#endif 
