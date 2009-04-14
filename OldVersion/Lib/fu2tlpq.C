#include <stdlib.h>
#include <iostream>
#include <values.h>
#include "math.h"
#include "fu2tlpq.h"
// $Id: fu2tl.C,v 1.9 2006/10/25 08:22:10 uid1029 Exp $ 


const char *Fu2tlpq::info()  {
    return "\
fu2tlpq '[opciones]'     Parecido al algoritmo de Fukunaga con dos hijos pero\n\
                          de forma que el hijo de la izquierda es el mismo\n\
			  que el padre y el de la derecha es el mas alejado.\n\
  -r [fsugt]             Reglas de eliminación : Fukunaga, Sibling, Ullman\n\
                          Generalized, Table\n\
  -dnn {double}          Distancia estimada al vecino mas cercano todos\n\
  -dnnf {double}         Distancia estimada al vecino mas cercano fukunaga\n\
  -dnnt {double}         Distancia estimada al vecino mas cercano tabla\n\
                          sirve para reducir la tabla de distancias\n\
  $Revision: 1.9 $\n";
  }

bool Fu2tlpq::fRegistrado = false;

static Fu2tlpq reg;

//-------------------------------------------------------------------
//      Lectura de parámetros
//

Fu2tlpq::Fu2tlpq( char data[] ) {
  char token[100];
  regla_f = false;
  regla_s = false;
  regla_u = false;
  regla_g = false;
  regla_t = false;
  table_type = 's';
  tabla = 0;
  dnnf = -1.0;
  dnnt = -1.0;


  double dnn = -1;
  int l;
  while( sscanf(data, "%s%n", token, &l) == 1 ) {
    data += l;
    // choosing the table 
    if( strcmp( token, "-dnn" ) == 0 ) {
      if( sscanf( data,"%lf%n", &dnn, &l ) == 0 || dnn < 0.0 ) {
	cerr << "ERROR en las opciones del algoritmo 'Fu2tlpq'" << endl;
	cerr << "  argumento de -dnn no es valido." << endl;
	exit(-1);
      }
      dnnf = dnn;
      dnnt = dnn;
      data += l;
    } 
    else if( strcmp( token, "-dnnf" ) == 0 ) {
      if( sscanf( data,"%lf%n", &dnnf, &l ) == 0 || dnnf < 0.0 ) {
	cerr << "ERROR en las opciones del algoritmo 'Fu2tlpq'" << endl;
	cerr << "  argumento de -dnnf no es valido." << endl;
	exit(-1);
      }
      data += l;
    } 
    else if( strcmp( token, "-dnnt" ) == 0 ) {
      if( sscanf( data,"%lf%n", &dnnt, &l ) == 0 || dnnt < 0.0 ) {
	cerr << "ERROR en las opciones del algoritmo 'Fu2tlpq'" << endl;
	cerr << "  argumento de -dnnt no es valido." << endl;
	exit(-1);
      }
      data += l;
    } 

   else  if( strcmp( token, "-r" ) == 0 ) {
      sscanf( data, "%s%n", token, &l );
      data += l;
      int len = strlen(token);
      for( int i = 0; i < len; i++ ) {
        if( token[i] == 'f' )
	  regla_f = true;
	else if( token[i] == 's' )
	  regla_s = true;
	else if( token[i] == 'g' )
	  regla_g = true;
	else if( token[i] == 't' )
	  regla_t = true;
	else if( token[i] == 'u' )
	  regla_u = true;
	else {
	  cerr << "ERROR en las opciones del algoritmo 'Fu2tlpq'" << endl;
	  cerr << "  regla '" << token[i] << "' desconocida." << endl;
	  exit(-1);
	}
      }
    } else {
      cerr << "ERROR en las opciones del algoritmo 'Fu2tlpq' " << endl;
      cerr << " opción '" << token << "' desconocida." << endl;
      exit(-1);
    }
  }
  raiz = 0;
 }

//-------------------------------------------------------------------

Fu2tlpq::Arbol *Fu2tlpq::construirArbol(       int p[], 
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
      cerr << "ERROR en Fu2tlpq::construirArbol(..)\n" << endl;
      cerr << "  no queda memoria" << endl;
      exit(-1);
    }
    if( repI != p[0] )
      cout << "distintos" << endl;
    a->numNodo = contNodos++;
    a->rep = p[0];
    a->radio_ext = 0; // not needed, done in the default constructor
    //    a->radio_int = ora->distancia(repI, p[0]); // se hace en el padre
    a->hijo_izq = 0; // not needed, done in the default constructor
    a->hijo_der = 0; // not needed, done in the default constructor

    return a;
  }

  // calcular el representante derecho (prototipo mas lejano)
  int repD = -1;
  disMax = -1.0;
  for( int i = 0 ; i< numP; i++ ) { 
    if( p[i] != repI ) {
      dis = ora->distancia(repI,p[i]);
      if( dis > disMax ) {
	disMax = dis;
	repD = p[i];
      }
    }
  }
  // disMax sera el radio del nodo
  
  // calcular distancias

  double dis_izq[numP];
  double dis_der[numP];

  for( int i = 0; i < numP; i++ ) {
    dis_izq[i] = ora->distancia(repI, p[i]); // podria evitar calcular esto
    dis_der[i] = ora->distancia(repD, p[i]);
  }


  // repartir los prototipos

  int pI[numP];
  int pD[numP];
  
  int numPI = 0;
  int numPD = 0;

  double sep = ora->distancia(repI, repD); // esto tambien podra evitarlo
  double radio_int_izq = sep;
  double radio_int_der = sep;

  for( int i = 0; i < numP; i++ ) {
    if( p[i] == repI )
      pI[numPI++] = i;
    else if( p[i] == repD )
      pD[numPD++] = i;
    else {
      if( dis_izq[i] < dis_der[i] ) {
	pI[numPI++] = i;
        if( dis_der[i] < radio_int_der )
	  radio_int_der = dis_der[i];	
       } else {
	pD[numPD++] = i;
	if( dis_izq[i] < radio_int_izq )
	  radio_int_izq = dis_izq[i];

      }
    }
  }

// despues de esto radio_int_der contiene la distancia mnima de los prototipos
// de la izquierda al representante de la derecha.
// lo mismo para radio_int_izq.
// La verdad es que el nombre es bastante desafortunado...

// pI contiene los prototipos de la izquierda y pD los de la derecha.


  int bor_izq[numPI];     // contendra los prototipos "borde" de la izquierda
  int num_bor_izq = 0;

  int bor_der[numPD];     // contendra los prototipos "borde" de la derecha
  int num_bor_der = 0;

  if( regla_g ) {

    double dis_max_der = MAXDOUBLE;
    while( true ) {
  
      double dis_max_izq = -1.0;
      for( int i = 0; i < numPI; i++ )
        if( dis_izq[pI[i]] > dis_max_izq && dis_der[pI[i]] < dis_max_der ) {
          dis_max_izq = dis_izq[pI[i]];
  	  bor_izq[num_bor_izq] = i;
        }
      
      if( dis_max_izq == -1.0 )
        break;
  
      dis_max_der = dis_der[pI[bor_izq[num_bor_izq]]];
      num_bor_izq++;
    }
      
// depues de esto, bor_izq[0], debe ser el prototipo de la izquierda
// mas alejado de su representante.
// Esto deberia coincidir con a->hijo_izq->radio_max 
// osea: dis_izq[pI[bor_izq[0]]] == a->hijo_izq->radio_max
// ahora no lo puedo comprobar...

  
    double dis_max_izq = MAXDOUBLE;
    while( true ) {
  
      double dis_max_der = -1.0;
      for( int i = 0; i < numPD; i++ )
        if( dis_der[pD[i]] > dis_max_der && dis_izq[pD[i]] < dis_max_izq ) {
          dis_max_der = dis_der[pD[i]];
  	  bor_der[num_bor_der] = i;
        }
      
      if( dis_max_der == -1.0 )
        break;
  
      dis_max_izq = dis_izq[pD[bor_der[num_bor_der]]];
      num_bor_der++;
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
    cout << "ERROR en Fu2tlpq::construirArbol(..)\n" << endl;
    cout << "  no queda memoria" << endl;
    exit(-1);
  }
  a->numNodo = contNodos++;
  a->rep = repI;
  a->radio_ext = disMax;
  a->hijo_izq = hi;
  a->hijo_der = hd;

  a->hijo_izq->radio_int = radio_int_izq;
  a->hijo_der->radio_int = radio_int_der;
  

  if( regla_g) {
    a->hijo_izq->num_bordes = num_bor_izq;
    if( num_bor_izq > 0 ) {
      a->hijo_izq->dis_su_rep = new double[num_bor_izq];
      if( a->hijo_izq->dis_su_rep == 0 ) {;
	cout << "ERROR en Fu2tlpq::construirArbol(..)\n" << endl;
	cout << "  no queda memoria" << endl;
	exit(-1);
      }
      a->hijo_izq->dis_otro_rep = new double[num_bor_izq];
      if( a->hijo_izq->dis_otro_rep == 0 ) {;
	cout << "ERROR en Fu2tlpq::construirArbol(..)\n" << endl;
	cout << "  no queda memoria" << endl;
	exit(-1);
      }
      
      for( int i = 0; i < num_bor_izq; i++ ) {
	a->hijo_izq->dis_su_rep[i] = dis_izq[pI[bor_izq[i]]]; // radio_ext
	a->hijo_izq->dis_otro_rep[i] = dis_der[pI[bor_izq[i]]];
      }
    }

    a->hijo_der->num_bordes = num_bor_der;
    if( num_bor_der > 0 ) {
      a->hijo_der->dis_su_rep = new double[num_bor_der];
      if( a->hijo_izq->dis_su_rep == 0 ) {;
	cout << "ERROR en Fu2tlpq::construirArbol(..)\n" << endl;
	cout << "  no queda memoria" << endl;
	exit(-1);
      }
      a->hijo_der->dis_otro_rep = new double[num_bor_der];
      if( a->hijo_izq->dis_otro_rep == 0 ) {;
	cout << "ERROR en Fu2tlpq::construirArbol(..)\n" << endl;
	cout << "  no queda memoria" << endl;
	exit(-1);
      }

      for( int i = 0; i < num_bor_der; i++ ) {
	a->hijo_der->dis_su_rep[i] = dis_der[pD[bor_der[i]]]; // radio_ext
	a->hijo_der->dis_otro_rep[i] = dis_izq[pD[bor_der[i]]];
      }
    }
  }

  return a;
}
    

Fu2tlpq::Arbol* Fu2tlpq::construirArbol() {
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
  contProto=0;
  contNodos = 0;
  return construirArbol(p,numP,masLejano);
}




//-------------------------------------------------------------------
//  Rellenar la tabla  
//

void Fu2tlpq::rellenarLinea(const int nodo, const Fu2tlpq::Arbol *a) {

  tabla->Set(nodo, a->numNodo , ora->distancia(nodo,a->rep));

  if( a->hijo_izq != 0 ) {
    rellenarLinea(nodo, a->hijo_izq);
    if( tabla->Get(nodo,a->hijo_izq->numNodo) < tabla->Get(nodo,a->numNodo) )
        tabla->Set(nodo,a->numNodo, tabla->Get(nodo,a->hijo_izq->numNodo));
  }
  if( a->hijo_der != 0 ) {
    rellenarLinea(nodo, a->hijo_der);
    if( tabla->Get(nodo,a->hijo_der->numNodo) < tabla->Get(nodo,a->numNodo) )
      tabla->Set(nodo,a->numNodo, tabla->Get(nodo,a->hijo_der->numNodo));
  }
}

//-----------------

void Fu2tlpq::rellenarTabla(){
  for(int nodo=0; nodo< np; ++nodo ){  
    rellenarLinea(nodo,raiz); 
  }   
}


//-------------------------------------------------------------------
//  Podar la tabla
//

void Fu2tlpq::podarLinea(const int nodo, const Fu2tlpq::Arbol *a) {

  celem++;

  if( dnnt >= 0.0 && 2.0 * dnnt > tabla->Get(nodo, a->numNodo) ) { 
//   NO SE APLICA LA TABLA
    tabla->Set(nodo, a-> numNodo, 0.0 );
    cpodas++;
  } 
  else

   if( dnnf >= 0.0 && regla_f && 
       dnnf < ora->distancia(nodo, a->rep) - a->radio_ext ) { 
//   SE APLICA FUKUNAGA asumiendo x=nn
    tabla->Set(nodo, a-> numNodo, 0.0 );
    cpodas++;
   }

  if( a->hijo_izq != 0 ){ 
    podarLinea(nodo, a->hijo_izq);
  if( a->hijo_der != 0 )
    podarLinea(nodo, a->hijo_der);

  }
}

//----------------- 

void Fu2tlpq::podarTabla(){
  celem = 0;
  cpodas = 0;
  for(int nodo=0; nodo< np; ++nodo ){  
    podarLinea(nodo,raiz); 
  }    
}

//-------------------------------------------------------------------
//    Cola de prioridad
//


struct PQueue_Node {
  Fu2tlpq::Arbol* a;
  double dis_izq;
  struct PQueue_Node *sig;
};


class PQueue {

public:
  PQueue(): q(0) {}

  void enqueue( Fu2tlpq::Arbol* a, double dis_izq ){

    PQueue_Node* p = new PQueue_Node;
    p->a = a;
    p->dis_izq = dis_izq;

    p->sig = q;
    q = p;
  }

  bool isvoid() {
    return q == 0;
  }

  bool dequeue( Fu2tlpq::Arbol* &a, double &dis_izq){ 

    PQueue_Node* ant = 0;
    PQueue_Node* p = q;
    if( p == 0 ) {
      return false;
    }
    PQueue_Node* p_min = p;
    PQueue_Node* p_ant = ant;
    ant = p;
    p = p->sig;
    while( p != 0 ) {
      if( p->dis_izq < p_min->dis_izq ) {
        p_min = p;
        p_ant = ant;
      }
      ant = p;
      p = p->sig;
    }
    if( p_ant == 0 )
      q = p_min->sig;
    else
      p_ant->sig = p_min->sig;

    a = p_min->a;
    dis_izq = p_min->dis_izq;

    delete p;

    return true;
  }
  
  private:

    PQueue_Node* q;
};

//-------------------------------------------------------------------
//    Buscar
//

bool gr_aplicable( double dis_izq, double dis_der, 
                   double dis_min, 
                   Fu2tlpq::Arbol* a) {
  int i; 

  if( dis_izq - dis_min < a->dis_su_rep[0] ) { 
    for( i = 1; i < a->num_bordes; i++ ) {
      if( dis_izq - dis_min > a->dis_su_rep[i] ) {
  	break;
      }
    }
    if(( dis_der + dis_min > a->dis_otro_rep[i-1] )) {
      return true;
    }
  }
  return false;
}
  
void Fu2tlpq::buscar( double &dis_min, int &mas_cercano) {
  nodosVisitados++;
  PQueue q;
  Arbol *a;

  double dis_izq = ora->distancia( raiz->rep );
  dis_min = dis_izq;
  mas_cercano = raiz->rep;
  q.enqueue(raiz,dis_izq);

  nodosVisitados++;

  while( q.dequeue( a, dis_izq ) ) {
    nodosVisitados++;

    if( a->hijo_izq == 0 && a->hijo_der == 0 ) {  // si estoy en una hoja
      continue;
    }

  // pruning what can be pruned without computing any new distance
  // if rules s, g or t are activated and right node can be pruned
  
    if( ((regla_s || regla_g) && 
          dis_izq + dis_min < a->hijo_izq->radio_int) || 
        (regla_t && 
          2.0 * dis_min < tabla->Get(mas_cercano,a->hijo_der->numNodo))
      ) {
        
      // rules f, g, t not activated or left node cannot be pruned
      if( (( !regla_f && !regla_g) || 
            dis_izq - dis_min < a->hijo_izq->radio_ext ) &&
 	  (!regla_t || 
	     2.0 * dis_min > tabla->Get(mas_cercano,a->hijo_izq->numNodo)) 
	  ){

        // looking only on the left --> sibling node has been pruned 
        q.enqueue( a->hijo_izq, dis_izq);
      }
      continue; // both were pruned.
    }     

    double dis_der = ora->distancia(a->hijo_der->rep);
    if( dis_der < dis_min ){
      dis_min = dis_der;
      mas_cercano = a->hijo_der->rep;
    }  
  
    // pruning and seaching if needed LEFT
    if(    (!regla_f || dis_izq - dis_min < a->hijo_izq->radio_ext ) 
        && (!regla_s || dis_der + dis_min > a->hijo_der->radio_int )
        && (!regla_u || dis_izq - dis_min < dis_der + dis_min )
        && (!regla_g || gr_aplicable(dis_izq, dis_der, dis_min, a->hijo_izq))
      ) {
        q.enqueue(a->hijo_izq, dis_izq);
    }
  // pruning and seaching if needed RIGHT
    if(    (!regla_f || dis_der - dis_min < a->hijo_der->radio_ext ) 
        && (!regla_s || dis_izq + dis_min > a->hijo_izq->radio_int ) 
        && (!regla_u || dis_izq + dis_min > dis_der - dis_min )
        && (!regla_g || gr_aplicable(dis_der, dis_izq, dis_min, a->hijo_der))
      ) {
      q.enqueue(a->hijo_der, dis_der);
    }
  }
}

//-------------------------------------------------------------------

int profundidad( Fu2tlpq::Arbol *a) {
  if( a == 0 )
    return 0;

  int p1 = profundidad( a->hijo_izq);
  int p2 = profundidad( a->hijo_der);
  if( p1 > p2 )
    return p1 + 1;
  else
    return p2 + 1;
}

