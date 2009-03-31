#ifndef oracle_h
#define oracle_h

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef int Point;

class Oracle {
public:

  virtual
  double distance( Point p1, Point p2 ) {
    cerr << "ERROR in Oracle::distance(const int, const int)" << endl;
    cerr << "  Method not implemented" << endl;
    exit(-1);
  }

  virtual
  Point newPoint() {
    cerr << "ERROR in Oracle::newPoint()" << endl;
    cerr << "  Method not implemented" << endl;
    exit(-1);
  }

  virtual
  void writePoint( Point p) {
    cerr << "ERROR in Oracle::writePoint()" << endl;
    cerr << "  Method not implemented" << endl;
    exit(-1);
  }

  virtual
  int distanceComputations() {
    cerr << "ERROR in Oracle::distanceComputations()" << endl;
    cerr << "  Method not implemented" << endl;
    exit(-1);
  }

/*
  virtual
  ~Oracle(){
    cerr << "ERROR in Oracle::~Oracle()" << endl;
    cerr << "  Method not implemented" << endl;
    exit(-1);
  }
*/

};

#endif
