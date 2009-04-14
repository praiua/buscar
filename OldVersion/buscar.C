#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <values.h>
#include <sys/resource.h>
#include <string.h>
#include <unistd.h>
#include "algBus.h"
#include "fuerzaBruta.h"

const double epsilon = 1E-10;

//--------------------------------------------------------------

double obtenerTiempo() {
  struct rusage usage;
  getrusage(RUSAGE_SELF,&usage);
  return usage.ru_utime.tv_sec + 0.000001 * usage.ru_utime.tv_usec;
}

//-------------------------------------------------------
//
// Programa principal
//

int main(int argc, char *argv[]) {
  int dim = 0;
  double numP = -1;
  double numPini = -1;
  double  numPstep = -1;

//Para reducir la tabla
  int numM = 0;
  bool com = false;
  bool coms = false;
  char *tipo_alg = 0;
  char *alg_data = 0;
  char *tipo_ora= 0;
  char *ora_data = 0;
//  int numPB = 0;
  int sem = 0;
  int rep = 1;
  bool incrExp = false;
  bool comprobar_dt= false;
  bool comprobar_dtm= false;
  bool cabecera = true;

  for( int i = 1; i < argc; i++ ) {
    if( strcmp(argv[i],"-dim") == 0 ) {
      dim = atoi(argv[++i]);
    } else if( strcmp(argv[i],"-pi") == 0 ) {
      numPini = atof(argv[++i]);
    } else if( strcmp(argv[i],"-ps") == 0 ) {
      numPstep = atof(argv[++i]);
    } else if( strcmp(argv[i],"-pf") == 0 ) {
      numP = atof(argv[++i]);
    } else if( strcmp(argv[i],"-pie") == 0 ) {
      incrExp = true;
    } else if( strcmp(argv[i],"-rep") == 0 ) {
      rep = atoi(argv[++i]);
    } else if( strcmp(argv[i],"-m") == 0 ) {
      numM = atoi(argv[++i]);
    } else if( strcmp(argv[i],"-alg") == 0 ) {
      tipo_alg = argv[++i];
      alg_data = argv[++i];
    } else if( strcmp(argv[i],"-ora") == 0 ) {
      tipo_ora = argv[++i];
      ora_data = argv[++i];
    } else if( strcmp(argv[i],"-sem") == 0 ) {
      sem = atoi(argv[++i]);
    } else if( strcmp(argv[i],"-com") == 0 ) {
      com = true;
    } else if( strcmp(argv[i],"-coms") == 0 ) {
      coms = true;
    } else if( strcmp(argv[i],"-cdt") == 0 ) {
      comprobar_dt= true; 
    } else if( strcmp(argv[i],"-cdtm") == 0 ) {
      comprobar_dtm= true; 
    } else if( strcmp(argv[i],"-noc") == 0 ) {
      cabecera = false;
    } else if( strcmp(argv[i],"-h") == 0 ) {
      cerr << "\
uso: buscar [opciones]\n\
\n\
  -pi  <p>                número innicial de prototipos\n\
  -pf  <p>                número final de prototipos\n\
  -ps  <p>                salto en el número de prototipos\n\
  -pie                    incremento exponencial [lineal]\n\
  -m   <p>                número de muestras\n\
  -alg <a> 'parametros'   algoritmo de búsqueda\n\
  -ora <o> 'parametros'   oráculo\n\
  -sem <s>                semilla\n\
  -com                    comprobar con la fuerza bruta\n\
  -coms                   comprobar con la fuerza bruta y sigue si error\n\
  -cdt                    comprobar desigualdad triangular\n\
  -cdtm			  comprobar desigualdad triangular en las muestras\n\
  -noc                    no imprimir las cabeceras\n\
  -h                      muestra esta ayuda\n\
  " << endl;

      cerr << "Algoritmos:" << endl;
      AlgoritmoBusqueda::escribirInfo();
      cerr << endl;
      cerr << "Oraculos:" << endl;
      Oraculo::escribirInfo();
      exit(0);
    }else {
      cerr << "Error: opción '"<< argv[i] << "' incorrecta." << endl;
      exit(-1);
    }
  }

  if( numP <= 0 ) {
    cerr << "Error: número de prototipos incorrecto ["<< numP <<"]" << endl;
    exit(-1);
  }

  if( numPini == -1 )
    numPini = numP;

  if( numPstep == -1 )
    numPstep = numP - numPini;

  if( numPstep == 0 )
    numPstep = 1;

  if( numPini < 0 || numPini > numP ) {
    cerr << "Error: número inicial de prototipos incorrecto ["
         << numPini <<"]" << endl;
    exit(-1);
  }

  if( numPstep <= 0 ) {
    cerr << "Error: incremento de prototipos inclorrecto ["
         << numPini <<"]" << endl;
    exit(-1);
  }

  if( numM <= 0 ) {
    cerr << "Error: número de muestras incorrecta ["<< numP <<"]" << endl;
    exit(-1);
  }

  if( tipo_alg == 0) {
    cerr << "Error: no se ha especificado el algoritmo de búsqueda" << endl;
    exit(-1);
  }

  if( tipo_ora == 0) {
    cerr << "Error: no se ha especificado el oráculo" << endl;
    exit(-1);
  }

// fijar Algoritmo

  AlgoritmoBusqueda *ab = AlgoritmoBusqueda::crear(tipo_alg,alg_data);
  if( ab == 0 ) {
    cerr << "Error: algoritmo '" << tipo_alg << "' desconocido." << endl;
    exit(-1);
  }

// fijar Oraculo

  Oraculo *o = Oraculo::crear(tipo_ora, ora_data);
  if( o == 0 ) {
    cerr << "Error: oraculo '" << tipo_ora << "' desconocido." << endl;
    exit(-1);
  }

  ab->fijarOraculo(o);

  o->fijarNumMues(numM);
  
//  cout << "# alg\tclust\tora\tdim\tsem\tnumP\tnumM\ttm\ttmp";
  if( cabecera )  {
    printf("# ");
    ab->escribirCabeceras();
    printf( " %8s %8s", "t", "t_pre");
    cout << endl;
  }

  FuerzaBruta * fb = new FuerzaBruta("");

  for( double j = numPini; j <= numP; (incrExp) ? j*=numPstep : j+=numPstep) {

// actualiza el oraculo

    double tiempo = 0;
    double tiempoPre = 0;
    
    for( int i = 0; i < rep; i++ ) {
    
      o->semilla(sem+i);
      o->fijarNumProt(int(j));
      o->preprocesar();

      double tiempoPre1 = obtenerTiempo();

      ab->preprocesar();

      tiempoPre += obtenerTiempo() - tiempoPre1;

      fb->fijarOraculo(o);
      fb->preprocesar();

      if( comprobar_dt ) {
        for( int i = 0; i < o->numProt(); i++ )
	  for( int j = 0; j < o->numProt(); j++ )
	    for( int k = 0; k < o->numProt(); k++ )
	      if( o->distancia(i,j) + o->distancia(j,k) + epsilon < o->distancia(i,k) ) {
		cerr << "La desigualdad triangular no se cumple" << endl;
		cout << "i: " << i << " j: " << j << " k: " << k << endl;
		cout << "distancia(" << i << "," << j << ") = " << o->distancia(i,j) << endl;
		cout << "distancia(" << j << "," << k << ") = " << o->distancia(j,k) << endl;
		cout << "distancia(" << i << "," << k << ") = " << o->distancia(i,k) << endl;
		exit(-1);
	      }
      }
      double tiempo1 = obtenerTiempo();

// realizar los experimentos

      double dis = 0.0;
      int masCercano = 0;
      int masCercano2 = 0;
      int iter = 0;

      do {
        ab->buscar(dis,masCercano);

	if( comprobar_dtm ) {
	  for( int i = 0; i < o->numProt(); i++ )
	    for( int j = 0; j < o->numProt(); j++ ) 
	      if( o->distancia(i,j) + o->distancia(j) < o->distancia(i) ) {
		cerr << "La desigualdad triangular no se cumple en las muestras" << endl;
		exit(-1);
	      }
        }

	iter++;
        if( com || coms ) {
          double dis2;
          fb->buscar(dis2,masCercano2);
 
	  if( dis != dis2 ) {
	    cerr << "Error en iter. " << iter << 
		    ": no coincide con la fuerza bruta" << endl;
	    cerr << ab->nombre()  
		 << ", prot: " << masCercano 
		 << " distancia: " << dis << endl;
	    cerr << "Fuerza bruta" 
		 << ", prot: " << masCercano2 
		 << " distancia: " << dis2 << endl;
            if(com)
	      exit(-1);
	  }
        }
      } while( o->nuevaMuestra() );
      double tiempo2 = obtenerTiempo();
      tiempo += tiempo2 - tiempo1;
    }

//    cout << ab->oraculo()->numProt() << endl;
    printf("  ");
    ab->escribirCaracteristicas();
    printf( " %8.2f %8.2f", tiempo, tiempoPre);
    cout << endl;

  }   
  delete o;
  delete ab;
  delete fb;

  return 0;
}
