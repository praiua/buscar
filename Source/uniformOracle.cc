
#include "uniformOracle.h"


CheckInOracle( 
  UniformOracle, 
  "unif", 
  "Uniform distribution\n\
  -d <int>   Set dimension"
)

//------------------------------------------------------
//
UniformOracle::UniformOracle( vector<string> data )
{
	mDim = -1;
	
	if( data.size() != 3 || data[1] != "-d" || StringToInt( data[2], mDim ) == false )
	{
		cerr << "ERROR (" << data[0] << "): Wrong input parameters" << endl;
		exit(-1);
	}
	
	if( mDim < 0 ) {
		cerr << "ERROR (" << data[0] << "): A dimension should be specified" << endl;
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
	for( int i = 0; i < mDim; i++ )
		cout << mData[p][i] << " ";
		
	cout << endl;
} 


//------------------------------------------------------
//
double UniformOracle::CalculateDistance( Point p1, Point p2 ) 
{
	double res = 0.0;

	for( int i = 0; i < mDim; i++ )
		res += pow( mData[p1][i] - mData[p2][i], 2.0);

	return sqrt(res);
}


