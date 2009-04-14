#include "oraDisL.h"
#include <math.h>
#include <fstream>
#include <string>

using namespace std;

bool OraDisL::fRegistrado = false;

static OraDisL reg;

OraDisL::OraDisL( char *data ) {
  prot = 0;
  mues = 0;
  n = 2.0;

  nProtMem = 0;
  nMuesMem = 0;
  dim = -1;
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
    } else if( strcmp( token, "-d" ) == 0 ) {
      sscanf( data, "%d%n", &dim, &l);
      data += l;
    } else if( strcmp( token, "-n" ) == 0 ) { // potencia
      sscanf( data, "%lf%n", &n, &l);
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

  if( dim <= 0 ) {
    cerr << "ERROR en las opciones del oraculo" << endl;
    cerr << "  ha de especificarse una dimension" 
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

  
  vector<double> v;
  double d;
  int cont = 0;
  while( f_prot >> d ) {
    v.push_back(d);
    cont++;
    if( cont == dim) {
      tabla.push_back(v);
      cont = 0;
      v.clear();
    }
  }

  if( cont != 0 ) {
    cerr << "ERROR al leer el fichero de prototipos:" << endl;
    exit(-1);
  }

  f_prot.close();
  nProtMem = tabla.size();

  while( f_mues >> d ) {
    v.push_back(d);
    cont++;
    if( cont == dim) {
      tabla.push_back(v);
      cont = 0;
      v.clear();
    }
  }

  if( cont != 0 ) {
    cerr << "ERROR al leer el fichero de prototipos:" << endl;
    exit(-1);
  }

  nMuesMem = tabla.size() - nProtMem;
  f_mues.close();

}

void OraDisL::preprocesar() {
  Oraculo::preprocesar();

  if( prot != 0 )
    delete prot;
  prot = new int[nProt];

  if( mues != 0 )
    delete mues;
  mues = new int[nMues];

  if( nMuesMem == 0 ) {   // lo saco todo de las muestras
    if( nProt + nMues > nProtMem ) {
      cerr << "ERROR en OraTabla1::preprocesar():" << endl;
      cerr << "  no hay bastantes cadenas (" << nMuesMem + nProtMem
	   << ") para " << nProt << " prototipos y " << nMues << " muestras."
	   << endl;
      exit(-1);
    }
  } else {
    if( nProt  > nProtMem ) {
      cerr << "ERROR en OraTabla1::preprocesar():" << endl;
      cerr << "  no hay bastantes prototipos (" << nProt
	   << ") en el fichero (" << nProtMem << ")."
	   << endl;
      exit(-1);
    }

    if( nMues  > nMuesMem ) {
      cerr << "ERROR en OraTabla1::preprocesar():" << endl;
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

double OraDisL::distancia( vector<double> v1, vector<double> v2 ) {

  double dis = 0.0;
  for( unsigned i = 0; i < v1.size(); i++ )
    dis += pow( fabs(v1[i] - v2[i]), n );

  return pow(dis, 1.0/n);
}

