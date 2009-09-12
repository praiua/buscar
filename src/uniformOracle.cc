
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
UniformOracle::UniformOracle( string data )
{
	mDim = -1;

        string token;
        istringstream ss(data);
        while( ss >> token ) {
                if( token == "-d" ) 
		{
                        if( !(ss >> mDim) )
                        {
                                cerr << "ERROR in UniformOracle: no dimension" << endl;
                                exit(-1);
                        }
		} 
		else
                {
                        cerr << "ERROR in UniformOracle: unrecognized option '" << token << "'" << endl;
                        exit(-1);
                }
 	}
	if( mDim <= 0 )
	{
		cerr << "ERROR in UniformOracle: no dimension given" << endl;
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


