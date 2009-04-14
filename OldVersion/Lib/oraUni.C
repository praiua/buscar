#include "oraUni.h"
#include <math.h>

bool OraUni::fRegistrado = false;

static OraUni reg;


OraUni::OraUni( char data[] ){
  prot = 0;
  mues = 0;
  dim = -1;
  dimI = -1;

  int l;
  char token[100];
  while( sscanf(data,"%s%n", token,  &l) == 1 ) {
    data += l;
    if( strcmp( token, "-d" ) == 0 ){
      sscanf( data, "%d%n", &dim, &l);
      data += l;
    } else if( strcmp( token, "-i" ) == 0 ){
      sscanf( data, "%d%n", &dimI, &l);
      data += l;
    } else {
      cerr << "ERROR en las opciones del oraculo" << endl;
      cerr << "  opcion '" << token << "' desconocida" << endl;
      exit(-1);
    }
  }

  if( dim < 0 ) {
    cerr << "ERROR: debe especificarse una dimensión" << endl;
    exit(-1);
  }
  if( dimI < 0 ) {
    dimI = dim;
  }
  if( dimI < 0 ) {
    cerr << "ERROR: el numero de dimensiones independientes" << endl;
    cerr << " ha de estar entre 1 y el número de dimensiones" << endl;
    exit(-1);
  }
}

void OraUni::preprocesar() {
  Oraculo::preprocesar();

  if( prot != 0 ) {
    for( int i = 0; i < nProtMem; i++ ) {
      delete prot[i];
    }
    delete prot;
  }

  if( mues != 0 ) {
    for( int i = 0; i < nMuesMem; i++ )
      delete mues[i];
    delete mues;
  }
    

  int dimD = dim - dimI;  // dimensiones dependientes
  double I[dimD];
  double A[dimD][dimI];
  for( int i = 0; i < dimD; i++ ) {
    I[i] = drand48();
    for( int j = 0; j < dimI; j++ )
//     A[i][j] = drand48();   
     A[i][j] = 50.0;   
  }

//  srand48(0);  // <=== NO TE OLVIDES DE QUITAR ESTA LINEA


  prot = new double*[nProt];
  mues = new double*[nMues];

//  cout << "dim: " << dim
//       << " dimI: " << dimI
//       << " dimD: " << dimD
//       << endl;

  for( int i = 0; i < nProt + nMues; i++ ){
    double* aux = new double[dim];
    for( int j = 0; j < dimI; j++ ) 
      aux[j] = drand48();
    for( int j = 0; j < dimD; j++ ) {
      aux[j + dimI] = I[j];
      for( int k = 0; k < dimI; k++ )
        aux[j + dimI] += A[j][k] * aux[k];
    }
    if( i < nProt )
      prot[i] = aux;
    else
      mues[i-nProt] = aux;
  }
  nProtMem = nProt;
  nMuesMem = nMues;

}    

double OraUni::distancia( double* v1, double* v2) {
  double res = 0.0;

  for( int i = 0; i < dim; i++ )
    res += pow( v1[i] - v2[i], 2.0);
  return sqrt(res);
}

OraUni::~OraUni(){
  if( prot != 0 ) {
    for( int i = 0; i < nProt; i++ )
      delete prot[i];
    delete prot;
  }

  if( mues != 0 ) {
    for( int i = 0; i < nMues; i++ )
      delete mues[i];
    delete mues;
  }
}
    
