#include <stdlib.h>
#include <iostream>
#include <values.h>
#include "fukunaga.h"
#include "oraculo.h"

bool Fukunaga::fRegistrado = false;

static Fukunaga reg;


void  comprobarCmeans( const int numHijos, 
                 const int numP, 
		 const int clase[], 
		 const int centroide[]) {

  // comprobar que para cada clase hay un centroide

  for( int i = 0; i < numHijos; i++ )
    if( clase[centroide[i]] != i ){
      cerr << "Error: el cmeans no funciona bien!" << endl;
      exit(-1);
    }

  // comprobar que solo hay numHijos clases

  int cont[numHijos];
  for( int i = 0; i < numHijos; i++ )
    cont[i] = 0;

  for( int i = 0; i < numP; ++i ) {
    if( clase[i] < 0 || clase[i] >= numHijos ) {
      cout << "Error: hay muchas clases" << endl;
      exit(-1);
    }
    cont[clase[i]]++;
  }

  for( int i = 0; i < numHijos; i++ ) {
    if( cont[i] < 1 ) {
      cerr << "Error: hay una clase demasiado pequeña" << endl;
      exit(-1);
    }
  }
}

int profundidad( Fukunaga::Arbol *a ) {
  if( a == 0 ) {
    return 0;
  }

  int prof_max = 0;
  for( int i = 0; i < a->num_hijos; i++ ) {
    int prof = profundidad(a->hijo[i]);
    if( prof > prof_max )
      prof_max = prof;
  }
  return prof_max + 1;
}

double calcularCentroide( const int c,
				Oraculo *ora,
                          const int prot[],
			  const int numP,
			  const int clase[],
			        int centroide[]) {

  // calcular la suma de distancia de cada prototipo a los demas de su clase

  double suma[numP];

  for( int i = 0; i < numP; i++ ) {
    suma[i] = 0.0;
    for( int j = 0; j < numP; j++ )
      if( i != j && clase[i] == clase[j] )
        suma[i] += ora->distancia(prot[i],prot[j]);
  }

  // el centroide de cada clase es el que minimiza la suma

  double min[c];
  for( int i = 0; i < c; i++ )
    min[i] = MAXDOUBLE;

  for( int i = 0; i < numP; i++)
    if( suma[i] < min[clase[i]] ) {
      min[clase[i]] = suma[i];
      centroide[clase[i]] = i;
    }

  // actualizar el critério de agrupamiento

  double J = 0.0;
  for( int i = 0; i < c; i++ )
    J += suma[centroide[i]];
  
  return J;
}

void cmeans( const int c,
	               Oraculo *ora,
             const int prot[],
             const int numP,
	           int clase[],
		   int centroide[]) {

  int cont[c];
  for( int i = 0; i < c; i++ )
    cont[i] = 0;

  // inicio al azar

  for( int i = 0; i < numP; i++ ){ 
    clase[i] = i%c;
    cont[clase[i]]++;
  }

  double J = calcularCentroide(c,ora,prot,numP,clase,centroide);

  // empieza el bucle

  int contador = 0; // cuenta el número de iteraciones sin cambios
  int i = 0; // indica el prototipo que estamos tratando
  do {

    if( cont[clase[i]] > 1 ) {
      int claseI = clase[i];

      double JMin = MAXDOUBLE;
      int iMin = -1;
      for( int j = 0; j < c; j++ ) { // pruebo con todas las clases
        if( clase[i] != j ) {  // que no sea la suya
	  clase[i] = j;
	  double Jaux = calcularCentroide(c,ora,prot,numP,clase,centroide);
	  if( Jaux < JMin ) {
	    JMin = Jaux;
	    iMin = j;
	  }
	}
      }
      if( JMin < J ) {
        J = JMin;
	clase[i] = iMin;
	cont[claseI]--;
	cont[iMin]++;
	contador = 0;
      } else {
        clase[i] = claseI;
      }
    }
    i = (i+1)%numP;
    contador++;

  } while( contador != numP);

  calcularCentroide(c,ora,prot,numP,clase,centroide);
}

void isodata( const int c,
	            Oraculo *ora,
              const int prot[],
              const int numP,
	            int clase[],
		    int centroide[]) {

  // inicio al azar

  for( int i = 0; i < numP; i++ ) {
    clase[i] = i%c;
  }

  bool cambio;
  do {
    cambio = false;
    calcularCentroide(c,ora,prot,numP,clase,centroide);
    for( int i = 0; i < numP; i++ ) {
      int claseCercana = -1;
      double disCercano = MAXDOUBLE;

      for( int j = 0; j < c; j++ ) {
        double dis = ora->distancia( prot[i], prot[centroide[j]]);
        if( dis < disCercano ) {
          disCercano = dis;
	  claseCercana = j;
        }
      }

      if( clase[i] != claseCercana ) {
        cambio = true;
        clase[i] = claseCercana;
      }
    }
  } while( cambio );

  calcularCentroide(c,ora,prot,numP,clase,centroide);
//  comprobarCmeans(c, numP, clase, centroide);

}

void maxsep(  const int c,
	                Oraculo *ora,
              const int prot[],
              const int numP,
	            int clase[],
		    int centroide[]) {

  int ini = prot[0];

  double disMax = -1.0;
  int protMax = -1;
  for( int i = 0; i < numP; i++ ) {
    double dis = ora->distancia(ini,prot[i]);
    if( dis > disMax) {
      disMax = dis;
      protMax = i;
    }
  }
  centroide[0] = protMax;

  double cota[numP];
  for( int i = 0; i < numP; i++ )
    cota[i] = MAXDOUBLE;

  for( int i = 1; i < c; i++ ) {
    double disMax = -1.0;
    int protMax = -1;
    for( int j = 0; j < numP; j++ ) {
      double dis = ora->distancia(prot[centroide[i-1]],prot[j]);
      if( cota[j] > dis )
        cota[j] = dis;
      if( cota[j] > disMax ) {
        disMax = cota[j];
	protMax = j;
      }
    }
    centroide[i] = protMax;
  }

  for( int i = 0; i < numP; i++ ) {
    double disMin = MAXDOUBLE;
    int claseMin = -1;
    for( int j = 0; j < c; j++ ) {
      double dis = ora->distancia(prot[centroide[j]],prot[i]);
      if( dis < disMin ) {
        disMin = dis;
	claseMin= j;
      }
    }
    clase[i] = claseMin;
  }
}

Fukunaga::Fukunaga( char data[] ) {
  char token[100];

  regla1 = false;
  regla2 = false;
  strcpy(alg_rep,"maxsep");
  num_hijos = 2;

  int l;
  while( sscanf( data, "%s%n", token, &l) == 1 ) {
    data += l;

    if( strcmp( token, "-ar") == 0 ) {
      sscanf( data, "%s%n", alg_rep, &l);
      data += l;
    } else if( strcmp( token, "-nh") == 0 ) {
      sscanf( data, "%d%n", &num_hijos, &l );
      data += l;
    } else if( strcmp( token, "-r") == 0 ) {
      sscanf( data, "%s%n", token,&l);
      data += l;
      for( unsigned int i = 0; i < strlen(token); i++ ) {
        if( token[i] == '1' )
	  regla1 = true;
        else if( token[i] == '2' )
	  regla2 = true;
	else {
	  cerr << "ERROR en opciones del algoritmo 'Fuku'" << endl;
	  cerr << "  regla '" << token[i] << "' desconocida." << endl;
	  exit(-1);
	}
      }
    } else {
      cerr << "ERROR en las opciones del algoritmo 'Fuku'." << endl;
      cerr << "opción '" << token << "' desconocida." << endl;
      exit(-1);
    }
  }

  raiz = 0;
  if( strcmp(alg_rep,"cmeans") == 0 )
    cluster = &cmeans;
  else if( strcmp(alg_rep,"isodata") == 0 ) 
    cluster = &isodata;
  else if( strcmp(alg_rep,"maxsep") == 0 )
    cluster = &maxsep;
  else {
    cerr << "Error: algoritmo de reparto desconocido" << endl;
    exit(-1);
  }
}

void Fukunaga::preprocesar() {
  AlgoritmoBusqueda::preprocesar();

  nodosVisitados = 0;

  if( raiz != 0)
    delete raiz;

  int num_p = ora->numProt();
  int p[num_p];

  for( int i = 0; i < num_p; i++) {
    p[i] = i;
  }

  raiz = construirArbol(p, num_p); // supongo que numP > numHijos
}
    
Fukunaga::Arbol *Fukunaga::construirArbol(       int p[], 
		                             const int num_p) {

  // ver el caso límite

  if( num_p < 0 ) {
    cerr << "ERROR en construirArbol(..): num_p < 0" << endl;
    exit(-1);
  }

  if( num_p == 0 ) {
    return 0;
  }


  Arbol *a = new Arbol;

  if( num_p < num_hijos ) {
    a->num_hijos = num_p;
    a->rep = new int[num_p];
    a->radio_ext = new double[num_p];
    a->radio_int = new double*[num_p];
    a->hijo = new Arbol*[num_p];
    for( int i = 0; i < num_p; i++ ) {
      a->rep[i] = p[i];
      a->radio_ext[i] = 0;
      a->radio_int[i] = new double[num_p];
      for( int j = 0; j < num_p; j++ )
        a->radio_int[i][j] = ora->distancia(p[i],p[j]);
      a->hijo[i] = 0;
    }
    return a;
  }

  a->num_hijos = num_hijos;
  a->rep = new int[num_hijos];
  a->radio_ext = new double[num_hijos];
  a->radio_int = new double*[num_hijos];
  for( int i = 0; i < num_hijos; i++ ) {
    a->radio_int[i] = new double[num_hijos];
    for( int j = 0; j < num_hijos; j++ )
      a->radio_int[i][j] = MAXDOUBLE;
  }

  a->hijo = new Arbol*[num_hijos];

  // agrupar los prototipos

  int clase[num_p]; // a que clase pertenece cada prototipo
  int centroide[num_hijos]; // indica cuales son los representantes

  cluster(num_hijos, ora, p, num_p, clase, centroide);

  // repartir los prototipos

  int pAux[num_p];

  for( int i = 0; i < a->num_hijos; i++ ) {
    
    int cont = 0;
    double disMax = 0.0;
    for( int j = 0; j < num_p; j++ )
      if( j != centroide[i] ) {
	double dis = ora->distancia(p[centroide[i]],p[j]);
	if( clase[j] == i ) {
	  pAux[cont++] = p[j];
	  if( dis > disMax ) {
	    disMax = dis;
	  }
	} else {
	  if( dis < a->radio_int[i][clase[j]] )
	    a->radio_int[i][clase[j]] = dis;
	}
      }

    a->rep[i] = p[centroide[i]];
    a->radio_ext[i] = disMax;
    a->hijo[i] = construirArbol(pAux, cont);
   
  }
/*
  for( int i = 0; i < a->num_hijos; i++ )
    for( int j = 0; j < a->num_hijos; j++ )
      if( i != j && a->radio_int[i][j] == MAXDOUBLE ) {
        cerr << "ERROR en construirArbol(..): radio_int[i][j] == MAXDOUBLE" 
	     << endl;
	exit(-1);
      }
*/
      
  return a;
}
    
    
void Fukunaga::buscar( const Fukunaga::Arbol  *a,
                             double &dis_min,
	                     int    &mas_cercano) {

  nodosVisitados++;

  double dis[a->num_hijos];
  bool usado[a->num_hijos];

  int cont = 0;
  for( int i = 0; i < a->num_hijos; i++ ) {

    dis[i] = ora->distancia(a->rep[i]);

    if( dis[i] < dis_min ) {
      dis_min = dis[i];
      mas_cercano = a->rep[i];
    }

    if( a->hijo[i] == 0 ) {
      usado[i] = true; // con esto indico que ya está usado
    } else {
      usado[i] = false;
      cont++;
    }
  }

// baja por los hijos siguiendo ese orden

  for( int i = 0; i < cont; i++ ) {

    double dm = MAXDOUBLE;
    int im = -1;
    for( int j = 0; j < a->num_hijos; j++ ) {
      if( !usado[j] ) {
        if( dm > dis[j] ) {
	  dm = dis[j];
	  im = j;
	}
      }
    }

    if( im == -1) {
      cerr << "ERROR en buscar(..): ind_min == -1" << endl;
      exit(-1);
    }

    if( !regla1 || dis[im] < dis_min + a->radio_ext[im] ) {

      bool elimino = false;
      if( regla2 ) {
	for( int j = 0; j < a->num_hijos; j++ )
	  if( j != im && dis[j] <= a->radio_int[j][im] - dis_min ) {
	    elimino = true;
	    break;
	  }
      }
      
      if( !elimino )
        buscar(a->hijo[im], dis_min, mas_cercano);

    }

    usado[im] = true;

  }

}


void Fukunaga::buscar(double &dis_min, int &mas_cercano) {
  dis_min = MAXDOUBLE;
  mas_cercano = -1;
  buscar(raiz,dis_min,mas_cercano);
}

