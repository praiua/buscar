//---------------------------------------------------------------
//
//  N.N. Algorithms Base Class
//

#ifndef nnAlg_h
#define nnAlg_h

#include <stdlib.h>
#include <iostream>
#include "oracle.h"


class NNAlg 
{
	public:
	
		virtual ~NNAlg(){};

		virtual void Insert( Point p ) = 0;

		virtual void InsertBulk( Point p[], int c ) = 0;

		virtual void SearchNN( Point p ) = 0;

		Point GetNNPoint() { return nnp; }

		double GetNNDistance() { return nnd; }	
		
	protected:
	
		Point nnp;   // the Nearest Neighbour Point
		double nnd;  // the nearest Neighbour Distance
		
		Oracle* mOracle;
		
};

#endif

