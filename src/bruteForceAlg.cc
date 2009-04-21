
#include "bruteForceAlg.h"


CheckInNNAlg(
  BruteForce,
  "bf",
  "Burte Force\n\
  No options"
)


//------------------------------------------------------
//
BruteForce::BruteForce( string data, Oracle * oracle )
{
	mOracle = oracle;
	
	if( !data.empty() )
	{
		cerr << "ERROR in BruteForce:  does not need options." << endl;
		cout << "Usage: " << endl;
		exit(-1);
	}
}


//------------------------------------------------------
//
void BruteForce::Insert( Point p ) 
{
	assert( (unsigned)p == mDb.size() );
	
	mDb.push_back( p );
}


//------------------------------------------------------
//
void BruteForce::InsertBulk( Point p[], int size) 
{		
	assert( size >= 0 );
	
	for( int i = 0; i < size; i++ ) 
	{
		assert( (unsigned)p[i] == mDb.size() );
		mDb.push_back( p[i] );
	}
}


//------------------------------------------------------
//
void BruteForce::SearchNN( Point p ) 
{
	assert( p >= 0);
	assert( (unsigned)p < mDb.size() );
	
	mNNPoint = mDb[0];
	mNNDistance = mOracle->GetDistance( p, mDb[0] );

	for( unsigned int i = 1; i < mDb.size(); i++) 
	{
		double dis = mOracle->GetDistance( p, mDb[i] );
		
		if( dis < mNNDistance ) {
			mNNDistance = dis;
			mNNPoint = mDb[i];
		}
	}
}

