#ifndef minkDistOracle_h
#define minkDistOracle_h

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include <assert.h>
#include "oracle.h"
#include "checkInOracle.h"
#include "lib_string.h"

using namespace std;



class MinkDistOracle : public Oracle 
{
	public:

		MinkDistOracle( vector<string> data );
		
		~MinkDistOracle();

		Point NewPoint();
		
		void PrintPoint( Point p );

		double CalculateDistance( Point p1, Point p2 );    


	private:

		void ReadPrototypes( string file );
		

		
		string mFileProt;
		string mOracleName;
		
		int mDim;
		float mDegree;
		
		vector< vector<double> > mTable;	// File content
		vector<int> mPrototypes;			// Prototype vector
		bool *mUsed;						// Used prototypes

};

#endif
