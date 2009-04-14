#include <stdlib.h>
#include <iostream>
#include <values.h>
#include "fu2.h"

bool Fu2::fRegistrado = false;

static Fu2 reg;

Fu2::Fu2( char data[] ) {
  char token[100];
  regla1 = false;
  regla2 = false;
  regla3 = false;
  equilibrado = false;
  repartiendo = false;

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
	else if( token[i] == '3' ) 
	  regla3 = true;
	else {
	  cerr << "ERROR en las opciones del algoritmo 'Fu2'" << endl;
	  cerr << "  regla '" << token[i] << "' desconocida." << endl;
	  exit(-1);
	}
      }

    } else if( strcmp(token, "-rep") == 0 ) {
      repartiendo = true;
    } else {
      cerr << "ERROR en las opciones del algoritmo 'Fu2' " << endl;
      cerr << " opción '" << token << "' desconocida." << endl;
      exit(-1);
    }
  }
  raiz = 0;
}


Fu2::Arbol *Fu2::construirArbol(       int p[], 
	  	                 const int numP) {

  // ver los casos límite

  if( numP < 0 ) {
    cerr << "Error: algo raro pasa" << endl;
    exit(-1);
  }

  if( numP == 0 ) {
    return 0;
  }

  if( numP == 1 ) {
    Arbol *a = new Arbol;
    a->rep_izq = p[0];
    a->rep_der = -1;

    a->radio_izq = 0;
    a->radio_der = 0;

    a->radio_int_izq = 0;
    a->radio_int_der = 0;

    a->hijo_izq = 0;
    a->hijo_der = 0;

    return a;
  }

  // calcular los dos representantes

  Arbol *a = new Arbol;

// el representante izquierdo es el prototipo mas lejano a un prototipo
// arbitrario (cojo el 0).

  double disMax = -1.0;
  for( int i = 1; i < numP; i++ ) {
    double dis = ora->distancia(p[0],p[i]);
    if( dis > disMax ) {
      disMax = dis;
      a->rep_izq = p[i];
    }
  }

  // el representante derecho el el prototipo mas lejano al representante
  // izquierdo

  disMax = -1.0;
  for( int i = 0; i < numP; i++ ) {
    if( p[i] != a->rep_izq ) {
      double dis = ora->distancia(a->rep_izq, p[i]);
      if( dis > disMax ) {
	disMax = dis;
	a->rep_der = p[i];
      }
    }
  }


  // repartir los prototipos

  int pI[numP];
  int pD[numP];

  int numPI = 0;
  int numPD = 0;

  if( repartiendo ) {

    double dis_izq[numP];
    double dis_der[numP];
    bool usado[numP];

    for( int i = 0; i < numP; i++ ) {
      if( p[i] == a->rep_izq || p[i] == a->rep_der )
        usado[i] = true;
      else  {
        dis_izq[i] = ora->distancia( a->rep_izq, p[i] );
        dis_der[i] = ora->distancia( a->rep_der, p[i] );
        usado[i] = false;
      }
    }

    a->radio_izq = 0.0;
    a->radio_der = 0.0;

    double sep = ora->distancia(a->rep_izq, a->rep_der);
    a->radio_int_izq = sep;
    a->radio_int_der = sep;

    bool va_izq = true;
    for( int i = 0; i < numP - 2; i++ ) {
      if( va_izq ) {
	int min = -1;
	double d_min = MAXDOUBLE;
	for( int j = 0; j < numP; j++ )
	  if( !usado[j] )
	    if( dis_izq[j] < d_min ) {
	      d_min = dis_izq[j];
	      min = j;
	    }
	pI[numPI] = p[min];
	numPI++;
	if( dis_izq[min] > a->radio_izq )
	  a->radio_izq = dis_izq[min];
	if( dis_der[min] < a->radio_int_der )
	  a->radio_int_der = dis_der[min];

	usado[min] = true;
      } else {
	int min = -1;
	double d_min = MAXDOUBLE;
	for( int j = 0; j != numP; j++ )
	  if( !usado[j] )
	    if( dis_der[j] < d_min ) {
	      d_min = dis_der[j];
	      min = j;
	    }
	pD[numPD] = p[min];
	numPD++;

	if( dis_der[min] > a->radio_der )
	  a->radio_der = dis_der[min];
	if( dis_izq[min] < a->radio_int_izq )
	  a->radio_int_izq = dis_izq[min];

	usado[min] = true;
      }
      va_izq = !va_izq;
    }

  } else {

// no incluyo los representantes en los subconjuntos por que ya he calculado
// sus distancias

    a->radio_izq = 0.0;
    a->radio_der = 0.0;

    double sep = ora->distancia(a->rep_izq, a->rep_der);
    a->radio_int_izq = sep;
    a->radio_int_der = sep;

    for( int i = 0; i < numP; i++ ) {
      if( p[i] != a->rep_izq && p[i] != a->rep_der ) {
	double dis_izq = ora->distancia(a->rep_izq, p[i]);
	double dis_der = ora->distancia(a->rep_der, p[i]);
	if( dis_izq < dis_der ) {
	  pI[numPI] = p[i];
	  numPI++;
	  if( dis_izq > a->radio_izq )
	    a->radio_izq = dis_izq;
	  if( dis_der < a->radio_int_der )
	    a->radio_int_der = dis_der;
	} else {
	  pD[numPD] = p[i];
	  numPD++;
	  if( dis_der > a->radio_der )
	    a->radio_der = dis_der;
	  if( dis_izq < a->radio_int_izq )
	    a->radio_int_izq = dis_izq;
	}
      }
    }
  }

  // llamada recursiva

  a->hijo_izq = construirArbol(pI,numPI);
  a->hijo_der = construirArbol(pD,numPD);

  return a;
}
    

Fu2::Arbol* Fu2::construirArbol() {
  int numP = ora->numProt();
  int p[numP];

  for( int i = 0; i < numP; i++) {
    p[i] = i;
  }

  return construirArbol(p,numP);
}
    
void Fu2::buscar( const Arbol  *a,
			double &dis_min,
			int    &mas_cercano) {

  if( a == 0 )
    return;

  nodosVisitados++;

  double dis_izq = ora->distancia(a->rep_izq);
  if( dis_izq < dis_min ) {
    dis_min = dis_izq;
    mas_cercano = a->rep_izq;
  }

  if( a->rep_der == -1 ) { // estoy en una hoja
    return;
  }

   // no aporta casi nada !!!

  if( regla3 && dis_izq < a->radio_int_izq - dis_min ) {
    buscar( a->hijo_izq, dis_min, mas_cercano);
    return;
  }


  double dis_der = ora->distancia(a->rep_der);
  if( dis_der < dis_min ) {
    dis_min = dis_der;
    mas_cercano = a->rep_der;
  }

  if( dis_izq < dis_der ) {

    if( ( !regla1 || dis_izq - dis_min < a->radio_izq  ) &&
        ( !regla2 || dis_der + dis_min > a->radio_int_der )  )
      buscar( a->hijo_izq,  dis_min, mas_cercano);

    if( ( !regla1 || dis_der - dis_min < a->radio_der ) &&
        ( !regla2 || dis_izq + dis_min > a->radio_int_izq )  )
      buscar( a->hijo_der,  dis_min, mas_cercano);

  } else {

    if( ( !regla1 || dis_der - dis_min < a->radio_der ) &&
        ( !regla2 || dis_izq + dis_min > a->radio_int_izq )  )
      buscar( a->hijo_der,  dis_min, mas_cercano);
    if( ( !regla1 || dis_izq - dis_min < a->radio_izq  ) &&
        ( !regla2 || dis_der + dis_min > a->radio_int_der )  )
      buscar( a->hijo_izq,  dis_min, mas_cercano);
  }

}

int profundidad( Fu2::Arbol *a) {
  if( a == 0 )
    return 0;

  int p1 = profundidad( a->hijo_izq);
  int p2 = profundidad( a->hijo_der);
  if( p1 > p2 )
    return p1 + 1;
  else
    return p2 + 1;
}
