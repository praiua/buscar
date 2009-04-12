
#include "minkDistOracle.h"


CheckInOracle( 
  MinkDistOracle,
  "mink",
  "Minkowski distance\n\
  -d <int>        Data dimension\n\
  -f <file>       File with prototypes\n\
  [-n <float>]    Distance degree\n\
  Note: The file of prototypes is a set of real numbers separated\n\
  by white spaces. In each line there is a vector."
)
 

//---------------------------------------------------------------------
//
MinkDistOracle::MinkDistOracle( vector<string> data ) 
{
	mDegree = 2.0;
	mDim = -1;
	bool error = false;
	mOracleName = data[0];
	

	for(unsigned int i = 1; i < data.size(); i++ )
	{
    	if( data[i] == "-f" && i < data.size() - 1)
		{
			mFileProt = data[++i];
		}
		else if( data[i] == "-d" && i < data.size() - 1 &&
			StringToInt(data[++i], mDim) )
		{
			;
		}
		else if( data[i] == "-n" && i < data.size() - 1 &&
			StringToFloat(data[++i], mDegree) )
		{
			;
		}
    	else 
    	{
    		error = true;			
		}
	}

	if( mFileProt.empty() || mDim <= 0)
	{
		error = true;
	}
	
	
	if( error )
	{
		cerr << "ERROR (" << data[0] << "): Wrong input parameters" << endl;
		cerr << "  Unknown '" << VectorToString( data ) << "' options" << endl;
		cout << "Usage: " << endl;
		CheckInOracle::ListInfo( data[0] );
		exit(-1);
	}
  

  	
	ReadPrototypes( mFileProt );

}


//---------------------------------------------------------------------
//
MinkDistOracle::~MinkDistOracle()
{
	delete [] mUsed;
}



//---------------------------------------------------------------------
//
void MinkDistOracle::ReadPrototypes( string file )
{
	ifstream fProt( file.c_str() );
	if( !fProt ) {
		cerr << "ERROR (" << mOracleName << "): The file '" << file << "' can not be opened." << endl;
		exit(-1);
	}
  
  	vector<double> v;
  	double d;
  	int cont = 0;
  	
  	while( fProt >> d ) 
  	{
    	v.push_back(d);
    	cont++;
    	if( cont == mDim) 
    	{
      		mTable.push_back(v);
      		cont = 0;
      		v.clear();
    	}
  	}
  	
  	fProt.close();
  	
  
  	if( mTable.size() == 0 || cont != 0) {
    	cerr << "ERROR (" << mOracleName << "): Wrong '" << file << "' file." << endl;
    	exit(-1);
  	}


 	mUsed = new bool[ mTable.size() ];	
	for( unsigned int i = 0; i < mTable.size(); i++ )
		mUsed[i] = false;

}



//---------------------------------------------------------------------
//
Point MinkDistOracle::NewPoint()
{

	if( mTable.size() == mPrototypes.size() ) 
	{   
		cerr << "ERROR (" << mOracleName << "): Not enough prototypes in the file '" << mFileProt << "'" << endl;
	    exit(-1);
	} 

	int randProt;
    	
    do {
    	randProt = int( drand48() * mTable.size() );
    
    } while( mUsed[ randProt ] );
    	
    mUsed[ randProt ] = true;
    mPrototypes.push_back( randProt );  	

  	return mPrototypes.size()-1;
}



//---------------------------------------------------------------------
//
void MinkDistOracle::PrintPoint( Point p )
{
	assert( p>=0 && (unsigned)p < mPrototypes.size() );
	assert( (unsigned)mPrototypes[ p ] < mTable.size() );
	
	vector<double> v = mTable[ mPrototypes[ p ] ];
	
	for(unsigned int i=0; i < v.size(); i++)
	{
    	cout << v[i] << " " << endl;
  	}
}





//---------------------------------------------------------------------
//
double MinkDistOracle::CalculateDistance( Point p1, Point p2 ) 
{
	assert( p1>=0 && (unsigned)p1 < mPrototypes.size() );
	assert( (unsigned)mPrototypes[ p1 ] < mTable.size() );
	assert( p2>=0 && (unsigned)p2 < mPrototypes.size() );
	assert( (unsigned)mPrototypes[ p2 ] < mTable.size() );
	
	vector<double> v1 = mTable[ mPrototypes[ p1 ] ];
	vector<double> v2 = mTable[ mPrototypes[ p2 ] ];	

	double dis = 0.0;
	
	for( unsigned i = 0; i < v1.size(); i++ )
		dis += pow( fabs(v1[i] - v2[i] ), mDegree );

	return pow(dis, 1.0/mDegree);
}

