//---------------------------------------------------------------
//
// Base clase for the Search Algorithm
//

#ifndef nnAlg_h
#define nnAlg_h

#include <stdlib.h>
#include <iostream>
//#include <string.h>
#include "oracle.h"


class NNAlg {
public:

  virtual
  void insert( Point p) = 0;

  virtual
  void insertBulk( Point p[], int c) = 0;

  virtual
  void NN( Point p) = 0;

  virtual
  Point nnPoint() = 0;

  virtual
  double nnDistance() = 0;
 
  virtual
  ~NNAlg(){};
};

#endif
