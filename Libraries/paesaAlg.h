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

		int mNp;              // nmero de prototipos
		int *mPivot;          // Pivotes
		double **mTDist;      // tabla de distancias
		//int mNum_used_pivots;
		//int mGpaso_mas_cercano;
		int mGRep;

};

#endif
