#include "oraTabla1.h"
#include <math.h>

bool OraTabla1::fRegistrado = false;

static OraTabla1 reg;

OraTabla1::OraTabla1( char data[] ) : tabla(0), prot(0), mues(0) {
  if( sscanf(data,"%s", fNomFich) != 1 ) {
    cerr << "ERROR: el argumento '" << data
	 << "' es inadecuado para el oraculo 'tab1'" << endl;
    exit(-1);
  }

  FILE* fp = fopen(fNomFich, "r");
  if( fp == 0 ) {
    cerr << "ERROR en OraTabla1::preprocesar():" << endl;
    cerr << "  no puedo abrir el fichero '" << fNomFich 
	 << "' para lectura" << endl;
    exit(-1);
  }

  if( fscanf(fp, "%d", &numPuntos) != 1 ) {
    cerr << "ERROR en OraTabla1::preprocesar():" << endl;
    cerr << "  file format error '" << endl; 
    exit(-1);
  }

  tabla = new double[numPuntos*numPuntos];
  if (!tabla) {
    cerr << "ERROR en OraTabla1::preprocesar():" << endl;
    cerr << "  no queda memoria (" << numPuntos*numPuntos*sizeof(double) << ")" <<  endl;
    exit(-1);    
  }
  
  for( int i = 0; i < numPuntos; i++ )
    for( int j = 0; j < numPuntos; j++ )
      if( fscanf(fp,"%lf", &tabla[i * numPuntos + j]) != 1 ){
        cerr << "ERROR en OraTabla1::preprocesar():" << endl;
        cerr << "  file format error '" << endl; 
        exit(-1);
      }

;

  fclose(fp);
/*
//   comprobar que es simetrica

  for( int p1 = 0; p1 < numPuntos; p1++ )
    for( int p2 = 0; p2 < numPuntos; p2++ )
      if( tabla[p1*numPuntos + p2] != tabla[p2*numPuntos + p1] ) {
        cerr << "ERROR: la distancia no es simetrica" << endl;
	exit(-1);
      }

  // comprobar la desigualdad triangular

  int cont = 0;
  for( int p1 = 0; p1 < numPuntos; p1++ )
    for( int p2 = 0; p2 < numPuntos; p2++ )
      for( int p3 = 0; p3 < numPuntos; p3++ ) {
        if( tabla[ p1 * numPuntos + p2] + tabla[p2*numPuntos + p3] <
	      tabla[p1 * numPuntos + p3] ) {
	  cont++;
	}
      }

  if( cont != 0 ) {
    cerr << "ERROR: no se cumple la desigualdad triangular en "
         << cont << " grupos de tres puntos " << endl;
    exit(-1);
  }
*/
}

void OraTabla1::preprocesar() {
  Oraculo::preprocesar();

  if( nMues + nProt >= numPuntos ) {
    cerr << "ERROR en OraTabla1::preprocesar():" << endl;
    cerr << "  no hay bastantes puntos (" << numPuntos
         << ") para " << nProt << " prototipos y " << nMues << " muestras."
	 << endl;
    exit(-1);
  }

  if( prot != 0 )
    delete prot;
  prot = new int[nProt];

  if( mues != 0 )
    delete mues;
  mues = new int[nMues];

  bool usado[numPuntos];
  for( int i = 0; i < numPuntos; i++ )
    usado[i] = false;

  // cout << "numPuntos: " << numPuntos << endl;
  // cout << "prototipos:" << endl;
  int az;
  for( int i = 0; i < nProt; i++ ) {
    do {
      az = int(drand48() * numPuntos);
    } while(usado[az]);
    // cout << az << endl;
    usado[az] = true;
    prot[i] = az;
  }

  // cout << "muestras:" << endl;
  for( int i = 0; i < nMues; i++ ) {
    do {
      az = int(drand48() * numPuntos);
    } while(usado[az]);
    // cout << az << endl;
    usado[az] = true;
    mues[i] = az;
  }

}    


