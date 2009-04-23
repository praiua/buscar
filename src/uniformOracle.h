#ifndef uniformOracle_h
#define uniformOracle_h

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "oracle.h"
#include "checkInOracle.h"
#include "lib_string.h"


class UniformOracle : public Oracle 
{
	public:

		UniformOracle( string data );

		Point NewPoint();
		
		void PrintPoint( Point p );
		
		double CalculateDistance( Point p1, Point p2 );
	  
	private:

		int mDim;
		vector< vector<double> > mData;

};

#endif
