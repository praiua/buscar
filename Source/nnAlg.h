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

		virtual void InsertBulk( Point p[], int size ) = 0;

		virtual void SearchNN( Point p ) = 0;

		Point GetNNPoint() { return mNNPoint; }

		double GetNNDistance() { return mNNDistance; }	
		
	protected:
	
		Point mNNPoint;   // the Nearest Neighbour Point
		
		double mNNDistance;  // the Nearest Neighbour Distance
		
		Oracle* mOracle;
		
};

#endif

