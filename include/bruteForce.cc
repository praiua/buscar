#include "bruteForce.h"
#include "checkInNNAlg.h"

CheckInNNAlg(
  BruteForce,
  "bf",
  "Burte Force\n\
   No options"
)

BruteForce::BruteForce( Oracle * oracle, const char data[]) {
  o = oracle;
  if( strcmp(data,"") != 0 )
    cerr << "ERROR: arguments are not valid" << endl;
}

void BruteForce::insert( Point p ) {
  db.push_back(p);
}

void BruteForce::insertBulk( Point p[], int c) {
  for( int i = 0; i < c; i++ ) {
    db.push_back(p[i]);
  }
}

void BruteForce::NN( Point p) {
  nnp = db[0];
  nnd = o->distance( p, db[0] );
  for( unsigned int i = 1; i < db.size(); i++) {
    double dis = o->distance( p, db[i]);
    if( dis < nnd ) {
      nnd = dis;
      nnp = db[i];
    }
  }
}

Point BruteForce::nnPoint() {
  return nnp;
}

double BruteForce::nnDistance() {
  return nnd;
}
