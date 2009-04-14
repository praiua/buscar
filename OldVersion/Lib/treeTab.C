#include <stdlib.h>
#include <iostream>
#include <values.h>
#include "treeTab.h"
#include "math.h"

bool TreeTab::fRegistrado = false;

static TreeTab reg;

TreeTab::TreeTab( char data[] ) {
  if( strcmp(data,"") != 0 ) {
    cerr << "ERROR: datos no válidos para este algoritmo" << endl;
    exit(-1);
  }
  raiz = 0;
  tabDis = 0;
  tabProt = 0;
};

TreeTab::Arbol *TreeTab::construirArbol(       int p[], 
		                         const int numP, 
		                         const int repI){

  double disMax = 0.0;
  double dis = 0.0;

  // ver el caso límite

  if( numP <= 0 ) {
    cerr << "Error: algo raro pasa" << endl;
    exit(-1);
  }

  if( numP == 1 ) {
    Arbol *a = new Arbol;
    if( a == 0 ) {
      cout << "ERROR en Fu2tl::construirArbol(..)\n" << endl;
      cout << "  no queda memoria" << endl;
      exit(-1);
    }
    if( repI != p[0] )
      cout << "distintos" << endl;
    a->rep = p[0];
    a->hijo_izq = 0;
    a->hijo_der = 0;

    return a;
  }

  // calcular el representante derecho (prototipo mas lejano)

  int repD = -1;
  disMax = -1.0;
  for( int i = 0; i < numP; i++ ) {
    if( p[i] != repI ) {
      dis = ora->distancia(repI,p[i]);
      if( dis > disMax ) {
	disMax = dis;
	repD = p[i];
      }
    }
  }

  // calcular distancias

  double dis_izq[numP];
  double dis_der[numP];

  for( int i = 0; i < numP; i++ ) {
    dis_izq[i] = ora->distancia(repI, p[i]);
    dis_der[i] = ora->distancia(repD, p[i]);
  }


  // repartir los prototipos

  int pI[numP];
  int pD[numP];
  
  int numPI = 0;
  int numPD = 0;

  for( int i = 0; i < numP; i++ ) {
    if( p[i] == repI )
      pI[numPI++] = i;
    else if( p[i] == repD )
      pD[numPD++] = i;
    else {
      if( dis_izq[i] < dis_der[i] ) {
	pI[numPI++] = i;
      } else {
	pD[numPD++] = i;
      }
    }
  }

  int pI2[numPI];
  for( int i = 0; i < numPI; i++ )
    pI2[i] = p[pI[i]];

  int pD2[numPD];
  for( int i = 0; i < numPD; i++ )
    pD2[i] = p[pD[i]];

  // llamada recursiva

  Arbol *hi = construirArbol(pI2,numPI,repI);
  Arbol *hd = construirArbol(pD2,numPD,repD);
      

  Arbol *a = new Arbol;
  if( a == 0 ) {
    cout << "ERROR en Fu2tl::construirArbol(..)\n" << endl;
    cout << "  no queda memoria" << endl;
    exit(-1);
  }
  a->rep = repI;
  a->hijo_izq = hi;
  a->hijo_der = hd;

  return a;
}
    

TreeTab::Arbol* TreeTab::construirArbol() {
  int numP = ora->numProt();
  int p[numP];
  double dis=0.0;

  for( int i = 0; i < numP; i++) {
    p[i] = i;
  }

  int masLejano = p[0];
  double disMasLejano = 0.0;
  for( int i = 1; i < numP; i++ ) {
    dis = ora->distancia(0,p[i]);
    if( dis > disMasLejano ) {
      disMasLejano = dis;
      masLejano = p[i];
    }
  }

  return construirArbol(p,numP,masLejano);
}
    
struct st {
  double dis;
  int    prot;
};


int comp( const void *e1, const void *e2) {
  const st *p1 = (const st*) e1;
  const st *p2 = (const st*) e2;
  if( p1->dis < p2->dis )
    return -1;
  if( p1->dis > p2->dis )
    return 1;
  return  0;
}

void TreeTab::construirTabla() {

  np = ora->numProt();
  tabDis = new double[np*np];
  tabProt = new int[np*np];

  st buf[np];

  for( int i = 0; i < np; i++ ) {
    for( int j = 0; j < np; j++ ) {
      buf[j].dis = ora->distancia(i,j);
      buf[j].prot = j;
    }

    qsort( buf, np, sizeof(st), comp );

    for( int j = 0; j < np - 1; j++ )
      if( buf[j].dis > buf[j+1].dis ) {
	cout << "error" << endl;
        exit(-1);
      }

    for( int j = 0; j < np; j++ ) {
      tabDis[i*np +j] = buf[j].dis;
      tabProt[i*np +j]  = buf[j].prot;
    }

  }

}

    
void TreeTab::buscarArbol(const Arbol  *a,
			        double &dis_min,
			        int    &mas_cercano) {

  double dis_izq = ora->distancia(a->hijo_izq->rep);
  mas_cercano = a->hijo_izq->rep;
  dis_min = dis_izq;
  buscarArbol(a, dis_izq, dis_min, mas_cercano);
}

void TreeTab::buscarArbol(const Arbol  *a,
		          const double dis_izq,
			        double &dis_min,
			        int    &mas_cercano) {


  if( a->hijo_izq == 0 && a->hijo_der == 0 ) {  // si estoy en una hoja 
    return;
  }

  nodosVisitados++;

  double dis_der = ora->distancia(a->hijo_der->rep);

  if( dis_der < dis_min ){
    dis_min = dis_der;
    mas_cercano = a->hijo_der->rep;
  }
  
  if( dis_izq < dis_der ) {
      buscarArbol(a->hijo_izq, dis_izq, dis_min, mas_cercano);
  } else {
      buscarArbol(a->hijo_der, dis_der, dis_min, mas_cercano);
  }

}

void TreeTab::buscarTabla( double &disMin, int &masCercano) {

  int i = masCercano;
  st buf[np];

  for( int j = 0; j < np; j++ ) {
    buf[j].dis = fabs( tabDis[i*np+j] - disMin );
    buf[j].prot = tabProt[i*np+j];
  }

  qsort( buf, np, sizeof(st), comp );

  for( int j = 0; j < np; j++ ) {
    int p = buf[j].prot;
    double d = ora->distancia(p);
    if( d < disMin ) {
      disMin = d;
      masCercano = p;
    }
    if( disMin < buf[j].dis )
      break;
  }
}

int profundidad( TreeTab::Arbol *a) {
  if( a == 0 )
    return 0;

  int p1 = profundidad( a->hijo_izq);
  int p2 = profundidad( a->hijo_der);
  if( p1 > p2 )
    return p1 + 1;
  else
    return p2 + 1;
}
