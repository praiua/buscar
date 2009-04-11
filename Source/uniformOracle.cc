
#include "uniformOracle.h"


CheckInOracle( 
  UniformOracle, 
  "unif", 
  "Uniform distribution\n\
  -d <int>        Set dimension\n\
######"
)


//------------------------------------------------------
//
UniformOracle::UniformOracle( vector<string> data )
{
	mDim = -1;
	
	if( data.size() != 3 || data[1] != "-d" || 
		!StringToInt( data[2], mDim ) || mDim <= 0 )
	{
		cerr << "ERROR (" << data[0] << "): Wrong input parameters" << endl;
		cout << "Usage: " << endl;
		CheckInOracle::ListInfo( data[0] );
		exit(-1);
	}
	
}    


//------------------------------------------------------
//
Point UniformOracle::NewPoint() 
{
	vector<double> v( mDim );
	
	for( int i = 0; i < mDim; i++ ) {
		v[i] = rand()/double(RAND_MAX);
	}
	mData.push_back(v);
	
	return mData.size()-1;
}


//------------------------------------------------------
//
void UniformOracle::PrintPoint( Point p ) 
{
	assert( p>=0 && (unsigned)p < mData.size() ); 
	
	for( int i = 0; i < mDim; i++ )
		cout << mData[p][i] << " ";
		
	cout << endl;
} 


//------------------------------------------------------
//
double UniformOracle::CalculateDistance( Point p1, Point p2 ) 
{
	assert( p1>=0 && (unsigned)p1 < mData.size() );
	assert( p2>=0 && (unsigned)p2 < mData.size() ); 
	
	double res = 0.0;

	for( int i = 0; i < mDim; i++ )
		res += pow( mData[p1][i] - mData[p2][i], 2.0);

	return sqrt(res);
}


