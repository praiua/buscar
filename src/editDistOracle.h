#ifndef EditDistOracle_h
#define EditDistOracle_h

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <string>
#include <assert.h>
#include "oracle.h"
#include "checkInOracle.h"
#include "lib_string.h"



class EditDistOracle : public Oracle 
{
	public:
		
		EditDistOracle( string data );
		
		~EditDistOracle();
		
		Point NewPoint();
		
		void PrintPoint( Point p );
		
		double CalculateDistance( Point p1, Point p2 );
		
		

	private:
	
		void ReadPrototypes( string file );


		string mFileProt;

		vector<string> mTable;		// File content
		vector<int> mPrototypes;  	// Prototype vector
		bool *mUsed;				// Used prototypes
		
		float mWi;		// Weights of insertion, deletion and substitution
		float mWd;
		float mWs;
};

#endif
