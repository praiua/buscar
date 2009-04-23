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
				
		
		string mAlg_pb;
		int mNum_pb;       // number of pb
		int mNum_p;	   // number of prototypes	
		int *mPb;          // pb
		int mNum_pnb;      // number of non-base
		bool *mEs_base;    // if is base
		//double **mDis_pb;  // table of distances
		vector< vector<double> > mDis_pb;  // table of distances
		
};

#endif 