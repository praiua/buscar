#ifndef paesa_h
#define paesa_h

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <limits>
#include <assert.h>
//#include <values.h>
#include <math.h>
#include "nnAlg.h"
#include "checkInNNAlg.h"
#include "lib_string.h"


class Paesa: public NNAlg 
{	
	public:

		Paesa( vector<string> data, Oracle * oracle );
		
		~Paesa();

		void Insert( Point p );
	
		void InsertBulk( Point p[], int size );
	
		void SearchNN( Point p );


	private:

		int np;              // nmero de prototipos
		int *pivot;          // Pivotes
		double **tdist;      // tabla de distancias
		int num_used_pivots;
		int gpaso_mas_cercano;
		int gRep;

};

#endif
