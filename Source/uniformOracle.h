#ifndef uniformOracle_h
#define uniformOracle_h

#include <iostream>
#include <vector>
#include "oracle.h"

class UniformOracle : public Oracle {
public:

  UniformOracle( const char data[]);

  double distance( Point p1, Point p2 );

  Point newPoint();

  void writePoint( Point p );

  int distanceComputations();
  
private:

  int nDist;
  int dim;
  vector< vector<double> > data;

};

#endif
