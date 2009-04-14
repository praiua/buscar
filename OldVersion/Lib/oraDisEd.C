#include "oraDisEd.h"
#include <math.h>
#include <fstream>
#include <string>

using namespace std;

bool OraDisEd::fRegistrado = false;

static OraDisEd reg;

OraDisEd::OraDisEd( char *data ) {
  prot = 0;
  mues = 0;

  nProtMem = 0;
  nMuesMem = 0;
  Pi = 1.0;
  Pb = 1.0;
  Ps = 2.0;
  fich_prot[0] = '\0';
  fich_mues[0] = '\0';

  int l;
  char token[100];
  while( sscanf(data, "%s%n", token, &l  ) == 1 ) {
    data += l;

    if( strcmp( token, "-f" ) == 0 ) {
      sscanf( data, "%s%n", fich_prot, &l);
      data += l;
    } else if( strcmp( token, "-fm" ) == 0 ) {
      sscanf( data, "%s%n", fich_mues, &l);
      data += l;
    } else if( strcmp(token, "-p" ) == 0 ) {
      sscanf( data, "%lf %lf %lf%n", &Pi, &Pb, &Ps, &l );
      data += l;
    } else {
      cerr << "ERROR en las opciones del oraculo" << endl;
      cerr << "  opción '" << token << "'desconocida" << endl;
      exit(-1);
    }
  }

  if( fich_prot[0] == '\0' ) {
    cerr << "ERROR en las opciones del oraculo" << endl;
    cerr << "  ha de especificarse el nombre de un fichero de prototipos" 
         << endl;
    exit(-1);
  }

  if( fich_mues[0] == '\0' ) {
    cerr << "ERROR en las opciones del oraculo" << endl;
    cerr << "  ha de especificarse el nombre de un fichero de muestras" 
         << endl;
    exit(-1);
  }

// leo los prototipos

  ifstream f_prot(fich_prot);

  if( !f_prot ) {
    cerr << "ERROR en las opciones del oraculo:" << endl;
    cerr << "  no puedo abrir el fichero '" << fich_prot
	 << "' para lectura" << endl;
    exit(-1);
  }

  ifstream f_mues(fich_mues);
  if( !f_mues  ) {
    cerr << "ERROR en las opciones del oraculo:" << endl;
    cerr << "  no puedo abrir el fichero '" << fich_mues
         << "' para lectura" << endl;
    exit(-1);
  }

  string cad;
  while( f_prot >> cad )
    tabla.push_back(cad);
  f_prot.close();
  nProtMem = tabla.size();

  while( f_mues >> cad ) 
    tabla.push_back(cad);
  nMuesMem = tabla.size() - nProtMem;
  f_mues.close();

}

void OraDisEd::preprocesar() {
  Oraculo::preprocesar();

  if( prot != 0 )
    delete prot;
  prot = new int[nProt];

  if( mues != 0 )
    delete mues;
  mues = new int[nMues];

  if( nMuesMem == 0 ) {   // lo saco todo de las muestras
    if( nProt + nMues > nProtMem ) {
      cerr << "ERROR en OraDisEd::preprocesar():" << endl;
      cerr << "  no hay bastantes cadenas (" << nMuesMem + nProtMem
	   << ") para " << nProt << " prototipos y " << nMues << " muestras."
	   << endl;
      exit(-1);
    }
  } else {
    if( nProt  > nProtMem ) {
      cerr << "ERROR en OraDisEd::preprocesar():" << endl;
      cerr << "  no hay bastantes prototipos (" << nProt
	   << ") en el fichero (" << nProtMem << ")."
	   << endl;
      exit(-1);
    }

    if( nMues  > nMuesMem ) {
      cerr << "ERROR en OraDisEd::preprocesar():" << endl;
      cerr << "  no hay bastantes muestras (" << nMues
	   << ") en el fichero (" << nMuesMem << ")."
	   << endl;
      exit(-1);
    }
  }

  bool usado[nProtMem + nMuesMem];
  for( int i = 0; i < nProtMem + nMuesMem; i++ )
      usado[i] = false;

  // cout << "nProtMem: " << nProtMem << endl;
  // cout << "prototipos:" << endl;
  int az;
  for( int i = 0; i < nProt; i++ ) {
    do {
      az = int(drand48() * nProtMem);
    } while(usado[az]);
    // cout << az << endl;
    usado[az] = true;
    prot[i] = az;
  }

  // cout << "muestras:" << endl;
  for( int i = 0; i < nMues; i++ ) {
    do {
      if( nMuesMem == 0 )
	az = int(drand48() * nProtMem);
      else
	az = nProtMem + int(drand48() * nMuesMem);
    } while(usado[az]);
    // cout << az << endl;
    usado[az] = true;
    mues[i] = az;
  }

}    

double OraDisEd::distancia( string cad1, string cad2 ) {

  int lCad1 = cad1.size();
  int lCad2 = cad2.size();

  double D[lCad1+1][lCad2+1];

  D[0][0] = 0.0;
  for( int i = 1; i <= lCad1; i++ )
    D[i][0] = D[i-1][0] + Pb;

  for( int j = 1; j <= lCad2; j++ ) {
    D[0][j] = D[0][j-1] + Pi;

    for( int i = 1; i <= lCad1; i++ ) {
      double aux;

      if( cad1[i] == cad2[j] )
        D[i][j] = D[i-1][j-1];
      else 
        D[i][j] = D[i-1][j-1] + Ps;

      aux = D[i-1][j] + Pb;
      if( aux < D[i][j] )
        D[i][j] = aux;

      aux = D[i][j-1] + Pi;
      if( aux < D[i][j] )
        D[i][j] = aux;
    }
  }

  return D[lCad1][lCad2] ;
// de esta forma no se cumple la desigualdad triangular ...
//  return D[lCad1][lCad2] / ( lCad1 + lCad2); 
}

