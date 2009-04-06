
#include "bruteForce.h"


CheckInNNAlg(
  BruteForce,
  "bf",
  "Burte Force\n\
  No options"
)


//------------------------------------------------------
//
BruteForce::BruteForce( vector<string> data, Oracle * oracle )
{
	mOracle = oracle;
	
	if( data.size() != 1 )
	{
		cerr << "ERROR: " << data[0] << " does not need options." << endl;
		exit(-1);
	}
}


//------------------------------------------------------
//
void BruteForce::Insert( Point p ) 
{
	db.push_back(p);
}


//------------------------------------------------------
//
void BruteForce::InsertBulk( Point p[], int c) 
{
	for( int i = 0; i < c; i++ ) {
		db.push_back(p[i]);
	}
}


//------------------------------------------------------
//
void BruteForce::SearchNN( Point p ) 
{
	nnp = db[0];
	nnd = mOracle->GetDistance( p, db[0] );

	for( unsigned int i = 1; i < db.size(); i++) 
	{
		double dis = mOracle->GetDistance( p, db[i]);
		
		if( dis < nnd ) {
			nnd = dis;
			nnp = db[i];
		}
	}
}

