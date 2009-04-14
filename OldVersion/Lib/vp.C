#include <stdlib.h>
#include <iostream>
#include <values.h>
#include "vp.h"

bool Vp::fRegistrado = false;

static Vp reg;

Vp::Vp( char data[] ) {
  char token[100];
  regla1 = false;
  regla2 = false;
  extremando = false;
  num_hijos = 2;

  int l;
  while( sscanf(data, "%s%n", token, &l) == 1 ) {
    data += l;

    if( strcmp( token, "-r" ) == 0 ) {
      sscanf( data, "%s%n", token, &l);
      data += l;
      for( unsigned int i = 0; i < strlen(token); i++ ) {
        if( token[i] == '1' )
	  regla1 = true;
	else if( token[i] == '2' ) 
	  regla2 = true;
	else {
	  cerr << "ERROR en las opciones del algoritmo 'Vp'" << endl;
	  cerr << "  regla '" << token[i] << "' desconocida." << endl;
	  exit(-1);
	}
      }

    } else if( strcmp(token, "-ext") == 0 ) {
      extremando = true;
    } else if( strcmp(token, "-nh") == 0 ) {
      sscanf( data, "%d%n", &num_hijos, &l);
      data += l;
    } else {
      cerr << "ERROR en las opciones del algoritmo 'Vp' " << endl;
      cerr << " opción '" << token << "' desconocida." << endl;
      exit(-1);
    }
  }
  raiz = 0;
}

    
void comprobar( const Vp::Arbol *a, bool *usado, int num_punt ) {

  if( a == 0 )
    return;

  if( a->rep >= num_punt || a->rep < 0 ) {
    cerr << "ERROR en comprobar: representante fuera de rango" << endl;
    exit(-1);
  }

  if( usado[a->rep] ) {
    cerr << "ERROR en comprobar: un representante aparece dos veces" << endl;
    exit(-1);
  }

  usado[a->rep] = true;

  for( int i = 0; i < a->num_hijos; i++ ) {
    comprobar(a->hijo[i], usado, num_punt);
  }
}


void comprobar( Vp::Arbol *a, int num_punt ) {
  bool usado[num_punt];

  for( int i = 0; i < num_punt; i++ )
    usado[i] = false;

  comprobar(a, usado, num_punt);

  for( int i = 0; i < num_punt; i++ )
    if( !usado[i] ) {
      cerr << "ERROR en comprobar: representante no usado" << endl;
      exit(-1);
    }
}

struct sdis {
  int p;
  double dis;
};

static int comp( const void *p1, const void *p2 ) {
  sdis *e1 = (sdis*) p1;
  sdis *e2 = (sdis*) p2;

  if( e1->dis < e2->dis )
    return -1;
  if( e1->dis > e2->dis )
    return 1;
  return 0;
}

Vp::Arbol *Vp::construirArbol(       int p[], 
	  	                 const int numP) {

  // ver los casos límite

  if( numP < 0 ) {
    cerr << "Error: algo raro pasa" << endl;
    exit(-1);
  }

  if( numP == 0 ) {
    return 0;
  }

  Arbol *a = new Arbol;
  a->num_hijos = num_hijos;
  a->radio_int = new double[num_hijos];
  a->radio_ext = new double[num_hijos];
  a->hijo = new Arbol*[num_hijos];

  // calcular el representante.
  
  // Si no se dice nada cojo el primero (el cero).

  double disMax = -1.0;
  a->rep = p[0];

  if( extremando ) {
  // si me interesa que sea un extremal
  // cojeré el mas lejano a un prototipo arbitrario (el 0)

    for( int i = 1; i < numP; i++ ) {
      double dis = ora->distancia(p[0],p[i]);
      if( dis > disMax ) {
	disMax = dis;
	a->rep = p[i];
      }
    }
  }
  // primero calculo la distancia de los prototipos al representante

  sdis dist[numP];
  int num_dis = 0;
  for( int i = 0; i < numP; i++ ) {
    if( p[i] != a->rep ) {
      dist[num_dis].dis = ora->distancia(p[i], a->rep);
      dist[num_dis].p = p[i];
      num_dis++;
    }
  }

  // voy a escojer un radio de forma que la mitad de los prototipos
  // disten menos que el radio del representante

  
  qsort( dist, num_dis, sizeof(sdis), comp);

  int total = 0;
  for( int i = 0; i < num_hijos; i++ ) {

    int p_bloque[num_dis/num_hijos + 1];

    int primero = i * num_dis/num_hijos;
    int ultimo  = ( i + 1 ) * num_dis/num_hijos;

    if( primero == ultimo ) { // bloque vacio
      a->radio_int[i] = 0; // por poner algo ...
      a->radio_ext[i] = 0;
      a->hijo[i] = 0;  // esto es lo importante
      continue;
    }

    if( ultimo <= 0 ) {
      cerr << "ERROR: ultimo: " << ultimo << endl;
      exit(-1);
    }

    a->radio_int[i] = dist[primero].dis;
    a->radio_ext[i] = dist[ultimo-1].dis;

    int cont = 0;
    for( int j = primero; j < ultimo; j++ ) {
      p_bloque[cont] = dist[j].p;
      cont++;
    }

    if( cont > num_dis/num_hijos + 1 ) {
      cerr << "ERROR: cont = " << cont  
           << " num_dis/num_hijos: " << num_dis / double(num_hijos)
	   << " primero: " << primero
	   << " ultimo: " << ultimo << endl;
      exit(-1);
    }

  // llamada recursiva

    a->hijo[i] = construirArbol(p_bloque, cont);
    total += cont;
  }

  if( total != num_dis ) {
    cerr << "ERROR en construirArbol(..): total != num_dis" << endl;
    exit(-1);
  }
  return a;
}
    

Vp::Arbol* Vp::construirArbol() {
  int numP = ora->numProt();
  int p[numP];
  Vp::Arbol* a;

  for( int i = 0; i < numP; i++) {
    p[i] = i;
  }

  a = construirArbol(p,numP);
//  comprobar(a,numP);
  return a;
}

void Vp::buscar( const Arbol  *a,
			double &dis_min,
			int    &mas_cercano) {

  if( a == 0 ) {
    cout << "ERROR en buscar(..): árbol vacio" << endl;
    exit(-1);
//    return;
  }

  nivel++;

  nodosVisitados++;

  double dis = ora->distancia(a->rep);
  if( dis < dis_min ) {
    dis_min = dis;
    mas_cercano = a->rep;
  }

  bool usado[num_hijos];

  int cont = 0;
  for( int i = 0; i < a->num_hijos ; i++ )
    if( a->hijo[i] == 0 ) 
      usado[i] = true;
    else {
      usado[i] = false;
      cont++;
    }

  for( int i = 0; i < cont; i++ ) {
    
    double min_dis = MAXDOUBLE;
    int min_hijo = -1;
    for( int j = 0; j < num_hijos; j++ ) {
      if( !usado[j] ) {
        double d;
	if( a->radio_int[j] < dis ) {
	  if( a->radio_ext[j] > dis )
	    d = 0.0;
	  else 
	    d = dis - a->radio_ext[j];
	} else {
	  d = a->radio_int[j] - dis;
	}
	if( d < min_dis ) {
	  min_dis = d;
	  min_hijo = j;
	}
      }
    }


    if( min_hijo == -1 ) {
      cerr << "ERROR en Vp::buscar(): min_hijo == -1" << endl;
      exit(-1);
    }

    usado[min_hijo] = true;
    if( ( !regla1 || dis - dis_min < a->radio_ext[min_hijo] ) &&
        ( !regla2 || dis + dis_min > a->radio_int[min_hijo] ) ) {
/*
      cout << nivel 
           << " rep: " << a->rep
           << " min_hijo: " << min_hijo
           << " min_dis: " << min_dis 
           << " dis: " << dis 
	   << " radio_ext: " << a->radio_ext[min_hijo]
	   << " radio_int: " << a->radio_int[min_hijo] << endl;
*/
      buscar( a->hijo[min_hijo], dis_min, mas_cercano);
    }
  }
  nivel--;
}

int profundidad( Vp::Arbol *a) {
  if( a == 0 )
    return 0;

  int max_p = 0;
  for( int i = 0; i < a->num_hijos; i++ ) {
    int p = profundidad( a->hijo[i]);
    if( p > max_p )
      p = max_p;
  }
  return max_p + 1;
}
