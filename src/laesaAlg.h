#ifndef laesa_h 
#define laesa_h


#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <limits>
#include <assert.h>
#include "nnAlg.h"
#include "checkInNNAlg.h"
#include "lib_string.h"


class Laesa: public NNAlg 
{
	
	public:
 
		Laesa( string data, Oracle *oracle );

		~Laesa();
  
		void Insert( Point p );
	
		void InsertBulk( Point p[], int size );
	
		void SearchNN( Point p );

  
	private:
	
//		void Min_sup();
		void Min_max();
				
		vector<int> mP;    // the prototypes
		string mAlgBp;    // Algorithm to choose the base prototypes
		int mMaxBp;       // maximum number of base prototypes
		int mNumBp;	   // current number of base prototypes	
		vector<int> mBp;   // the index of the base prototypes
//		int mNum_pnb;      // number of non-base
                vector<bool> mIsBase; // register of base prototypes
		// table of distances:
		// - first index: index of the prototype
		// - second index: index of the base prototype
		vector< vector<double> > mBpDis; 
		
};

#endif 
