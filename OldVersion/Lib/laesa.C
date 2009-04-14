#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <values.h>
#include "laesa.h"
#include "oraculo.h"

bool Laesa::fRegistrado = false;

static Laesa reg;

Laesa::Laesa( char data[] ) {
  char token[100];

  num_pb = 2;
  strcpy(apb,"minmax");

  int l;
  while( sscanf( data, "%s%n", token, &l) == 1 ) {
    data += l;

    if( strcmp( token, "-pb") == 0 ) {
      sscanf( data, "%d%n", &num_pb, &l);
      data += l;
    } else if( strcmp( token, "-apb") == 0 ) {
      sscanf( data, "%s%n", apb, &l);
      data += l;
    } else {
      cerr << "ERROR en las opciones del algoritmo 'laesa'." << endl;
      cerr << "opción '" << token << "' desconocida." << endl;
      exit(-1);
    }
  }

  pb = new int[num_pb];
  dis_pb = new double*[num_pb];
  for( int i = 0; i < num_pb; i++ )
    dis_pb[i] = 0;
  es_base = 0;
}

static void  min_sup( int num_p, int num_pb, double **dis_pb, int *pb, 
                      bool *es_base, Oraculo *ora ) {

  double dis[num_p][num_p];

  for( int i = 0; i < num_p; i++ )
    for( int j = 0; j < num_p; j++ )
      dis[i][j] = ora->distancia(i,j);

  double dmc[num_p];
  for( int i = 0; i < num_p; i++ ) {

    dmc[i] = MAXDOUBLE;
    for( int j = 0; j < num_p; j++ ) {
      if( j != i ) {
        double d = dis[i][j];
	if( d < dmc[i] )
	  dmc[i] = d;
      }
    }

  }

  int vivos[num_p][num_p];
  for( int i = 0; i < num_p; i++ )
    for( int j = 0; j < num_p; j++ )
      vivos[i][j] = j;
  int n_vivos[num_p];
  for( int i = 0; i < num_p; i++ )
    n_vivos[i] = num_p;

  for( int b = 0; b < num_pb; b++ ) {
    int cont_min = MAXINT;
    int i_cont_min = -1;
    for( int i = 0; i < num_p; i++ ) {
      int cont = 0;

      for( int j = 0; j < num_p; j++ ) {
        for( int k = 0; k < n_vivos[j]; k++ ) {
	  if( fabs(dis[i][j] - dis[i][vivos[j][k]]) <= dmc[j] ) 
	    cont++;
	}
      }

      if( cont < cont_min ) {
	cont_min = cont;
	i_cont_min = i;
      }
    }
    pb[b] = i_cont_min;
    for( int j = 0; j < num_p; j++ ) {
      int cont = 0;
      for( int k = 0; k < n_vivos[j]; k++ ) {
        if( fabs( dis[i_cont_min][j] - dis[i_cont_min][vivos[j][k]])<=dmc[j] ){
	  vivos[j][cont] = vivos[j][k];
	  cont++;
	}
      }
      n_vivos[j] = cont;
    }
    
//    cout << cont_min / double(n_prot) << endl;
  }

  for( int i = 0; i < num_pb; i++ )
    for( int j = 0; j < num_p; j++ )
      dis_pb[i][j] = dis[pb[i]][j];

  for( int i = 0; i < num_p; i++ )
    es_base[i] = false;
  for( int i = 0; i < num_pb; i++ ) {
//    cout << pb[i] << endl;
    es_base[pb[i]] = true;
  }
}

static void  min_max( int num_p, int num_pb, double **dis_pb, int *pb, 
                      bool *es_base, Oraculo *ora ) {

// inicio el vector de distancias mínimas

  double d_min[num_p];

  for( int i = 0; i < num_p; i++ ) {
    d_min[i] = MAXDOUBLE;
    es_base[i] = false;
  }

  int i = 0;
  // el primer prototipo base es el '0'
  pb[0] = 0;
  es_base[0] = true;

  while( true ) {
  
  // relleno la tabla de distancias y acutalizo el vector de distancias
  // mínimas

    for( int j = 0; j < num_p; j++ ) {
      dis_pb[i][j] = ora->distancia(pb[i], j);
      if( dis_pb[i][j] < d_min[j] )
        d_min[j] = dis_pb[i][j];
    }
    
    i++;
    if( i == num_pb )
      break;

  // busco el siguiente prototipo base

    double max = -1.0;
    for( int j = 0; j < num_p; j++ ) {
      if( !es_base[j] && d_min[j] > max ) {
        max = d_min[j];
	pb[i] = j;
      }
    }

    if( max == -1.0) {
      cerr << "ERROR en Laesa::Preprocesar(..): " << endl
           << "  se han acabado los prototipos" << endl ;
      exit(-1);
    }

    es_base[pb[i]] = true;
  }
}

void Laesa::preprocesar() {
  AlgoritmoBusqueda::preprocesar();

  num_p = ora->numProt();

// reservo espacio para la tabla de distancias

  for( int i = 0; i < num_pb; i++ ) {
    delete dis_pb[i];
    dis_pb[i] = new double[num_p];
  }

  if( es_base != 0 )
    delete es_base;
  es_base = new bool[num_p];

  if( strcmp(apb,"minmax") == 0 )
    min_max( num_p, num_pb, dis_pb, pb, es_base, ora );
  else if( strcmp(apb,"minsup") == 0 )
    min_sup( num_p, num_pb, dis_pb, pb, es_base, ora );
  else {
    cerr << "ERROR: algoritmo de prototipos base '" << apb 
         << "' desconocido" << endl;
    exit(-1);
  }

  num_pnb = num_p - num_pb;
//  cout << "num_pb :" << num_pb << endl;
//  cout << "num_pnb:" << num_pnb << endl;
}
    
/*
void Laesa::buscar(double &dis_min, int &mas_cercano) {

  dis_min = MAXDOUBLE;
  mas_cercano = -1;

  int  num_p = ora->numProt();
  double g[num_p];
  bool usado[num_p];

  for( int i = 0; i < num_p; i++ ) {
    g[i] = 0.0;
    usado[i] = false;
  }

// calculo distancias a los prototipos base

  for( int i = 0; i < num_pb; i++ ) {
    double dis = ora->distancia(pb[i]);
    if( dis < dis_min ) {
      dis_min = dis;
      mas_cercano = pb[i];
    }
    usado[pb[i]] = true;
    g[pb[i]] = dis;
  }

// actualizo las cotas inferiores (g)

  for( int j = 0; j < num_p; j++ ) {
    if( !usado[j] ) {
      for( int i = 0; i < num_pb; i++ ) {
	double dif = fabs( g[pb[i]] - dis_pb[i][j]);
	if( dif > dis_min ) {  // elimino
	  usado[j] = true;
	  break;
	}
	if( dif > g[j] )
	  g[j] = dif;
      }
    }
  }

  while( true ) {
    
// busco un candidato a mas cercano
    
    double g_min = MAXDOUBLE;
    int ind_g_min = -1;

    for( int i = 0; i < num_p; i++ )
      if( !usado[i] && g[i] < g_min ) {
        g_min = g[i];
	ind_g_min = i;
      }

// si no lo encuentro, termino.

    if( ind_g_min == -1 )
      break;

// calculo la distancia

    double dis = ora->distancia(ind_g_min);
    if( dis < dis_min ) {
      dis_min = dis;
      mas_cercano = ind_g_min;
    }
    usado[ind_g_min] = true;

// elimino (solo si dis_min decrece, pero creo que se puede evitar)

    for( int i = 0; i < num_p; i++ )
      if( !usado[i] && g[i] > dis_min )
        usado[i] = true;

  }

}
*/

struct Elem {
  double g;
  int    ind;
};

static int comp( const void* elem1, const void* elem2 ) {
  if( ((Elem*)elem1)->g < ((Elem*)elem2)->g )
    return -1;
  if( ((Elem*)elem1)->g > ((Elem*)elem2)->g )
    return 1;
  return 0;
  
}

void Laesa::buscar(double &dis_min, int &mas_cercano) {

  dis_min = MAXDOUBLE;
  mas_cercano = -1;

  double dis_pb_x[num_pb];

// calculo distancias a los prototipos base

  for( int i = 0; i < num_pb; i++ ) {
    double dis = ora->distancia(pb[i]);
    if( dis < dis_min ) {
      dis_min = dis;
      mas_cercano = pb[i];
    }
    dis_pb_x[i] = dis;
  }

// actualizo las cotas inferiores (g)

  Elem elem[num_pnb];

  int cont = 0;
  for( int j = 0; j < num_p; j++ ) {
    if( !es_base[j] ) {
      elem[cont].g = 0.0;
      elem[cont].ind = j;
      for( int i = 0; i < num_pb; i++ ) {
	double dif = fabs( dis_pb_x[i] - dis_pb[i][j]);
	if( dif > elem[cont].g )
	  elem[cont].g = dif;
      }
      cont++;
    }
  }

  if( cont != num_pnb ) {
    cerr << "ERROR en laesa::buscar(..)" << endl;
    cerr << "  'cont != num_pnb' " << endl;
    cerr << "cont   : " << cont << endl;
    cerr << "num_pnb: " << num_pnb << endl;
    exit(-1);
  }
    

  qsort( elem, num_pnb, sizeof(Elem), comp );

  for( int i = 0; i < num_pnb; i++ ) {
    
    if( elem[i].g > dis_min )
      break;

// calculo la distancia

    double dis = ora->distancia(elem[i].ind);
    if( dis < dis_min ) {
      dis_min = dis;
      mas_cercano = elem[i].ind;
    }

  }

}

