#include <values.h>
#include "aesa.h"

bool Aesa::fRegistrado = false;
static Aesa reg;

Aesa::Aesa(const char data[]){
  if( strcmp(data,"") != 0 )
    cerr << "ERROR: parámetros no válidos para este algoritmo" << endl;

  tdist = 0;
  np = -1;

};

void Aesa::preprocesar() {
  AlgoritmoBusqueda::preprocesar();


  if( tdist != 0 ) {
    for( int i = 0; i < np; i++ )
      delete[] tdist[i];
    delete[] tdist;
  }


  np = ora->numProt();
    
  tdist = new double*[np];
  for( int i = 0; i < np; i++ ) {
    tdist[i] = new double[np];
    for( int j = 0; j < np; j++ )
      tdist[i][j] = ora->distancia(i,j);
  }

  gpaso_mas_cercano = 0;
}


void Aesa::buscar( double &disMin, int &masCercano) {
  bool eliminado[np];
  double G[np];
  for( int i = 0; i < np; i++ ) {
    eliminado[i] = false;
    G[i] = 0;
  }

  int paso = 0;
  int paso_mas_cercano = 0;

  masCercano = -1;
  disMin = MAXDOUBLE;

  int piv = 0;
  bool quedan_no_eliminados = true;
  while( quedan_no_eliminados ) {
    paso++;

    double dis_piv_mues = ora->distancia(piv);
    if( dis_piv_mues < disMin ) {
      disMin = dis_piv_mues;
      masCercano = piv;
      paso_mas_cercano = paso;
    }
    eliminado[piv] = true;

    for( int i = 0; i < np; i++ ) { // Actualizar cota
      double dis_cir = fabs( tdist[piv][i] - dis_piv_mues );
      if( G[i] < dis_cir )
        G[i] = dis_cir;
    }


    for( int i = 0; i < np; i++ ) { // Eliminación
      if( G[i] >= disMin ) 
        eliminado[i] = true;
    }
     
    double min_G = MAXDOUBLE;
    piv = -1;
    for( int i = 0; i < np; i++ ) { // Aproximación
      if( !eliminado[i] && G[i] < min_G ) {
        min_G = G[i];
        piv = i;
      }
    }
    if( piv == -1 )
      quedan_no_eliminados = false;

  }
  gpaso_mas_cercano += paso_mas_cercano;
}


