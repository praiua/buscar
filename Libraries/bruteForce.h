#ifndef bruteForce_h
#define bruteForce_h

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "nnAlg.h"
#include "checkInNNAlg.h"


class BruteForce: public NNAlg 
{
	public:

		BruteForce( vector<string> data, Oracle * oracle );

		void Insert( Point);

		void InsertBulk( Point p[], int size );

		void SearchNN( Point p );
		

	private:
		
		vector<Point> mDb;

};

#endif
