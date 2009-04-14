#include <values.h>
#include "paesa.h"

bool Paesa::fRegistrado = false;
static Paesa reg;

Paesa::Paesa(const char data[]){
  tdist = 0;
  np = -1;
  gRep = 1;

  char token[100];
  int l;
  while( sscanf( data, "%s%n", token, &l) == 1 ) {
    data += l;

    if( strcmp(token, "-r") == 0 ) {
      sscanf( data, "%d%n", &gRep,&l);
      data += l;
    } else {
      cerr << "ERROR en las opciones del algoritmo 'paesa'." << endl;
      cerr << "opción '" << token << "' desconocida." << endl;
      exit(-1);
    }
  }

};

void Paesa::preprocesar() {
  AlgoritmoBusqueda::preprocesar();


  if( tdist != 0 ) {
    for( int i = 0; i < np; i++ )
      delete[] tdist[i];
    delete[] tdist;
    delete[] pivot;
  }


  np = ora->numProt();
    
  tdist = new double*[np];
  for( int i = 0; i < np; i++ ) {
    tdist[i] = new double[np];
    for( int j = 0; j < np; j++ )
      tdist[i][j] = ora->distancia(i,j);
  }
/*
  pivot = new int[np];
  bool eliminado[np];
  double dis_pivots[np];
  for( int i = 0; i < np; i++) {
    eliminado[i] = false;
    dis_pivots[i] = 0.0;
  }
  
  int piv = 0;

  for( int k = 0; k < np; k++ ) {

    pivot[k] = piv;
    eliminado[k] = true;

    for( int i = 0; i < np; i++ )  // Actualización de la separacion a los piv
      if( !eliminado[i] )
        dis_pivots[i] += tdist[piv][i];

    double max = 0.0;
    for( int i = 0; i < np; i++ ) // Esoger el mejor pivot
      if( !eliminado[i] && dis_pivots[i] > max ) {
        max = dis_pivots[i];
        piv = i;
      }
    
  }
*/

  pivot = new int[np];
  bool eliminado[np];
  double min_dis_pivots[np];
  for( int i = 0; i < np; i++) {
    eliminado[i] = false;
    min_dis_pivots[i] = MAXDOUBLE;
  }

  int piv = 0;

  for( int k = 0; k < np; k++ ) {

    pivot[k] = piv;
    eliminado[k] = true;

    for( int i = 0; i < np; i++ ) // Actualización de la separacion a los piv
      if( !eliminado[i] && min_dis_pivots[i] > tdist[piv][i] )
        min_dis_pivots[i] = tdist[piv][i];

    double max = 0.0;
    for( int i = 0; i < np; i++ ) // Esoger el mejor pivot
      if( !eliminado[i] && min_dis_pivots[i] > max ) {
        max = min_dis_pivots[i];
        piv = i;
      }
    
  }

  for( int i = 0; i < np; i++ )
    if( pivot[i] < 0 || pivot[i] >= np )
      cerr << "ERROR: incorrect pivot" << endl;

  num_used_pivots = 0;
  gpaso_mas_cercano = 0;
/*
  int* pivot2 = new int[np];
  int j = 0;
  for( int i = 1; i < np; i += 2 )
    pivot2[j++] = pivot[i];
  for( int i = 0; i < np; i += 2 )
    pivot2[j++] = pivot[i];
  
  delete[] pivot;
  pivot = pivot2;
*/
}


void Paesa::buscar( double &disMin, int &masCercano) {
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

  double min_G = 0;
  double min_G_ant = 0.0;
  int piv = -1;

  int cont = 0;
  for( int j = 0; j < np; j++) {
    paso++;

    num_used_pivots++;
    piv = pivot[j];

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

    min_G_ant = min_G;
    min_G = MAXDOUBLE;
    piv = -1;
    for( int i = 0; i < np; i++ ) { // Aproximación
      if( !eliminado[i] && G[i] < min_G ) {
        min_G = G[i];
        piv = i;
      }
    }
    if( min_G <= min_G_ant ) {
      cont++;
    } else {
      cont = 0;
    }

    if( cont == gRep ) 
      break;
  }


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
     
    double min = MAXDOUBLE;
    piv = -1;
    for( int i = 0; i < np; i++ ) { // Aproximación
      if( !eliminado[i]) {
        double aux =  G[i];
        if( aux < min ) {
          min = aux ;
          piv = i;
        }
      }
    }

    if( piv == -1 )
      quedan_no_eliminados = false;

  }
  gpaso_mas_cercano += paso_mas_cercano;
}


