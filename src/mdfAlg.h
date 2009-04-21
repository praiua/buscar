#ifndef mdf_h
#define mdf_h
 
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <assert.h>
#include "nnAlg.h"
#include "checkInNNAlg.h"
#include "lib_string.h"



class Mdf: public NNAlg 
{
  public:

	Mdf( string data, Oracle *_o );

	~Mdf();
	
	void Insert(Point p);

	void InsertBulk( Point p[], int c);

	void SearchNN(Point p);

	

// protected:

	class Tree 
	{
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

	void ExtractPoints( Mdf::Tree *t, vector<Point> &db);

	void Insert( Point p, Mdf::Tree* &t, double disToRep);

	Mdf::Tree* BuildTree( Point lRep,
		                vector<Point> db,
		                vector<double> dLRep,
		                double minRadius );

	void SearchNN( const Mdf::Tree  *t,
		         double disToLeftRep);

	void Print( Mdf::Tree *t);
  
  
private:

	Point qp;     // The query point
	Tree *root;
	bool fRule;
	bool sRule;
	bool uRule;

};

#endif

