#ifndef mdf_h
#define mdf_h
 
#include "nnAlg.h"

class Mdf: public NNAlg {
public:

  Mdf( Oracle *_o, const char data[]);

  void insert(Point p);

  void insertBulk( Point p[], int c);

  void NN(Point p);
 
  Point nnPoint();

  double nnDistance();

  ~Mdf();

// protected:

  class Tree {
  public:
    Tree(): lChild(0), rChild(0) {};

    ~Tree() {
      if( lChild != 0)
        delete lChild;
      if( rChild != 0)
        delete rChild;
    }

    Tree *lChild;
    Tree *rChild;
    Point rep;
    double radius; // radius 
    double minRadius; // distacia al mas cercano del hermano

  };

  void extractPoints( Mdf::Tree *t, vector<Point> &db);

  void insert( Point p, Mdf::Tree* &t, double disToRep);

  Mdf::Tree* buildTree( Point lRep,
                        vector<Point> db,
                        vector<double> dLRep,
                        double minRadius );

  void NN( const Mdf::Tree  *t,
                 double disToLeftRep);

  void print( Mdf::Tree *t);
private:

  Point qp;     // The query point
  Point nnp;    // The Nearest Neighbour Point
  double nnd;   // The Nearest Neighbour Distance
  Oracle* o;
  Tree *root;
  bool fRule;
  bool sRule;
  bool uRule;

};

#endif
