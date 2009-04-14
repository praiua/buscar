#ifndef laesa_h 
#define laesa_h

//#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <limits>
#include <assert.h>
//#include <values.h>
#include "nnAlg.h"
#include "checkInNNAlg.h"
#include "lib_string.h"


class Laesa: public NNAlg 
{
	
	public:
 
		Laesa( vector<string> data, Oracle *oracle );

		~Laesa();
  
		void Insert( Point p );
	
		void InsertBulk( Point p[], int size );
	
		void SearchNN( Point p );		

  
	private:
	
		void Min_sup();
		void Min_max();
				
		
		string mAlg_pb;
		int mNum_pb;
		int mNum_p;		
		int *mPb;
		int mNum_pnb;
		bool *mEs_base;
		double **mDis_pb;
		
};

#endif 
