#ifndef aesa_h
#define aesa_h

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <math.h>
#include <assert.h>
#include "nnAlg.h"
#include "checkInNNAlg.h"
#include "lib_string.h"


class Aesa: public NNAlg 
{
	public: 

		Aesa( string data, Oracle *oracle );
	  
		void Insert( Point p );
	
		void InsertBulk( Point p[], int size );
	
		void SearchNN( Point p );
		

 
	private:		

		vector< vector<double> > mTDist;	// Table of distances
};

#endif
