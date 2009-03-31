#ifndef bruteForce_h
#define bruteForce_h

#include <vector>
#include "nnAlg.h"

class BruteForce: public NNAlg {
public:

  BruteForce( Oracle *o, const char data[]);

  void insert( Point);

  void insertBulk( Point p[], int c);

  void NN( Point p);

  Point nnPoint(); 

  double nnDistance();

private:

  Point nnp;   // the Nearest Neighbour Point
  double nnd;  // the nearest Neighbour Distance
  Oracle* o;
  vector<Point> db;
};

#endif
