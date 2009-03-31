#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <values.h>
#include "mdf.h"
#include "checkInNNAlg.h"


CheckInNNAlg(
  Mdf,
  "mdf",
  "Most Distant of the Father Algorithm\n\
  -r [fsu]      Pruning rules: Fukunaga, Sibling, Ullman"
)

Mdf::~Mdf() {
  delete root;
}

//-------------------------------------------------------------------
//    Options set up
//-------------------------------------------------------------------

Mdf::Mdf( Oracle *_o, const char data[] ) {
  o = _o;
  istringstream opts(data);

  string token;
  fRule = false;
  sRule = false;
  uRule = false;

  while( opts >> token ) {
    if( token == "-r" ) {
      if( !(opts >> token) ) {
	cerr << "ERROR in the options of the NN Algorithm 'Mdf'" << endl;
	cerr << "  -r argument not valid." << endl;
	exit(-1);
      }
      int len = token.length();
      for( int i = 0; i < len; i++ ) {
        if( token[i] == 'f' )
	  fRule = true;
	else if( token[i] == 's' )
	  sRule = true;
	else if( token[i] == 'u' )
	  uRule = true;
	else {
	  cerr << "ERROR in the options of the NN Algorithm 'Mdf'" << endl;
	  cerr << "  unknown '" << token[i] << "' rule." << endl;
	  exit(-1);
	}
      }
    } else {
      cerr << "ERROR in the options of the NN Algorithm 'Mdf'" << endl;
      cerr << "  unknown '" << token << "' option." << endl;
      exit(-1);
    }
  }
  root = 0;

}

//-------------------------------------------------------------------
// Insert Bulk
//-------------------------------------------------------------------

void Mdf::insertBulk( Point _db[], int c) {

  if( root != 0 )
    delete root;

  vector<Point> db( _db+1, _db + c );
  vector<double> dLRep(db.size());

  int lRep = _db[0];
  for( unsigned int i = 0; i < db.size(); i++ ) {
    dLRep[i] = o->distance(lRep, db[i]);
  }

  root = buildTree( lRep, db, dLRep, 0.0 );

  // print( root );
}

//-------------------------------------

Mdf::Tree *Mdf::buildTree(  Point lRep, 
                            vector<Point> db,
		            vector<double> dLRep,
                            double minRadius
                          ){

  //
  // ---- Base Case ----
  //
  if( db.size() == 0 ) {
    Tree *t = new Tree;
    if( t == 0 ) {
      cerr << "ERROR in Mdf::buildTree(..)" << endl;
      cerr << "  memory exhausted" << endl;
      exit(-1);
    }
    t->rep = lRep; 
    t->radius = 0; // not needed, done in the default constructor
    t->minRadius = minRadius; // it is done in the father
    t->lChild = 0; // not needed, done in the default constructor
    t->rChild = 0; // not needed, done in the default constructor

    return t;
  }
  // 
  // ---- Inductive Step ----
  //
  //  Computing Right Representant and node radius
  //

//  cout << "p.size(): " << p.size() << endl;
  double radius = -1.0;
  int  pRRep = -1;
  for( unsigned int i = 0; i < db.size(); i++ ) {
    if( dLRep[i] > radius ) {
      radius = dLRep[i];
      pRRep = i;
    }
  }

  Point rRep = db[pRRep];
  db.erase(db.begin() + pRRep );
  dLRep.erase(dLRep.begin() + pRRep );

  //
  // Computing Distances to Right Representant
  //

  vector<double> dRRep(db.size());
  for( unsigned int i = 0; i < db.size(); i++ ) {
    dRRep[i] = o->distance(rRep,db[i]);
  } 

  // distribute the points by its NN

  vector<Point> lP;   // elements of p that are going to go to the left
  vector<double> dLP;
  vector<Point> rP;   // elements of p that are going to go to the right
  vector<double> dRP;
  
  double minRadiusL = radius;
  double minRadiusR = radius;

  for( unsigned int i = 0; i < db.size(); i++ ) {
    if( dLRep[i] < dRRep[i] ) {
      lP.push_back(db[i]);
      dLP.push_back(dLRep[i]);
      if( dRRep[i] < minRadiusR )
        minRadiusR = dRRep[i];
    } else {
      rP.push_back(db[i]);
      dRP.push_back(dRRep[i]);
      if( dLRep[i] < minRadiusL )
        minRadiusL = dLRep[i];
    }
  }

  // recursive call

  Tree *lTree = buildTree( lRep, lP, dLP, minRadiusL );
  Tree *rTree = buildTree( rRep, rP, dRP, minRadiusR );
      
  Tree *t = new Tree;
  if( t == 0 ) {
    cout << "ERROR in Mdf::buildTree(..)\n" << endl;
    cout << "  memory exhausted" << endl;
    exit(-1);
  }
  t->rep = lRep;
  t->radius = radius;
  t->lChild = lTree;
  t->rChild = rTree;
  t->minRadius = minRadius;

  return t;
}
    
//-------------------------------------------------------------------
// Insert Incrementally
//-------------------------------------------------------------------

void Mdf::insert( Point p) {

  if( root == 0 ) { // only used when inserting the first point
    vector<Point> db;
    vector<double> dP;
    root = buildTree( p, db, dP, 0.0 ); 
    return;
  }

  double disToLRep = o->distance(root->rep, p);
  insert(p, root, disToLRep);
  // cout << "-- Printing tree --" << endl;
  // print( root );
}

//----------------------
//
void Mdf::insert( Point p, Mdf::Tree* &t, double disToLRep ) {
  
  Point lRep = t->rep;
  if( disToLRep > t->radius ) {
    vector<Point> db;
    vector<double> dLRep;

    extractPoints( t, db );
    for( unsigned int i = 0; i < db.size(); i++ ) {
      dLRep.push_back( o->distance( lRep, db[i]));
    }
    db.push_back(p);
    dLRep.push_back(disToLRep);
    double minRad = t->minRadius;
    delete t;

    t = buildTree( lRep, db, dLRep, minRad ); // CAMBIALO
    return;
  } 

  if( t->lChild == 0 ) { 
    if( t->rChild != 0 ) {
      cout << "ERROR in Mdf::insert(..)" << endl;
      cout << "  this shouldn't happend" << endl;
      exit(-1);
    }
    vector<Point> db;
    vector<double> dP;

    db.push_back(p);
    dP.push_back(disToLRep);
    double minRad = disToLRep;
    delete t;
    t = buildTree(lRep, db, dP, minRad); 
    return;
  }

  double disToRRep = o->distance( p, t->rChild->rep );
  if( disToLRep < disToRRep ) {
    insert( p, t->lChild, disToLRep );
    if( disToRRep < t->rChild->minRadius )
      t->rChild->minRadius = disToRRep;
  } else {
    insert( p, t->rChild, disToRRep );
    if( disToLRep < t->lChild->minRadius )
      t->lChild->minRadius = disToLRep;
  }
}

//-------------------
//
void Mdf::extractPoints( Mdf::Tree* t, vector<Point> &db) {
  if( t->lChild != 0 ) {
    extractPoints(t->lChild, db);
  }
  if( t->rChild != 0 ) {
    db.push_back(t->rChild->rep);
    extractPoints(t->rChild, db);
  }
}

//-------------------------------------------------------------------
//    Search
//-------------------------------------------------------------------

void Mdf::NN( const Mdf::Tree  *t,
	      double disToLeftRep ) {

  if( t->lChild == 0 && t->rChild == 0 ) {  // if I'm in a leave
    return;
  }

  // pruning what can be pruned without computing any new distance
  // if rules s, g or t are activated and right node can be pruned
  if( sRule && disToLeftRep + nnd < t->lChild->minRadius ) {
        
      // rule f not activated or left node cannot be pruned
    if( !fRule || disToLeftRep - nnd < t->lChild->radius ){
        // looking only on the left --> sibling node has been pruned 
        NN( t->lChild, disToLeftRep);
    }
    return; // both were pruned.
  }    

  double disToRightRep = o->distance( qp, t->rChild->rep);
  if( disToRightRep < nnd ){
    nnd = disToRightRep;
    nnp = t->rChild->rep;
  }
  
  if( disToLeftRep < disToRightRep ) {
    // pruning and seaching if needed LEFT
    if(    (!fRule || disToLeftRep - nnd < t->lChild->radius ) 
        && (!sRule || disToRightRep + nnd > t->rChild->minRadius )
        && (!uRule || disToLeftRep - nnd < disToRightRep + nnd )
      ) {
        NN( t->lChild, disToLeftRep);
    }
    // pruning and seaching if needed RIGHT
    if(    (!fRule || disToRightRep - nnd < t->rChild->radius ) 
        && (!sRule || disToLeftRep + nnd > t->lChild->minRadius ) 
        && (!uRule || disToLeftRep + nnd > disToRightRep - nnd )
      ) {
      NN( t->rChild, disToRightRep );
    }
  } else {
    // pruning and seaching if needed RIGHT
    if(    (!fRule || disToRightRep - nnd < t->rChild->radius ) 
        && (!sRule || disToLeftRep + nnd > t->lChild->minRadius ) 
        && (!uRule || disToLeftRep + nnd > disToRightRep - nnd )
      ) {
      NN( t->rChild, disToRightRep );
    }
    // pruning and seaching if needed LEFT
    if(    (!fRule || disToLeftRep - nnd < t->lChild->radius )
        && (!sRule || disToRightRep + nnd > t->rChild->minRadius ) 
        && (!uRule || disToLeftRep - nnd < disToRightRep + nnd )
      ) {
      NN( t->lChild, disToLeftRep );
    }
  }
}

void Mdf::NN( Point _qp) {

  qp = _qp;
  nnp = root->rep;
  nnd = o->distance( qp, root->rep );

  return NN( root, nnd );
}

//-------------------------------------------------------------------
//  Output
//-------------------------------------------------------------------

Point Mdf::nnPoint() {
  return nnp;
}

double Mdf::nnDistance() {
  return nnd;
}

//-------------------------------------------------------------------
//  Auxiliary
//-------------------------------------------------------------------

void Mdf::print( Mdf::Tree *t) {
  if( t != 0 ) {
    cout << "rep:" << t->rep << endl;
    cout << "radius: " << t->radius << endl;
    cout << "minRadius: " << t->minRadius << endl;
    cout << endl;
    print( t->lChild );
    print( t->rChild );
  }
}
