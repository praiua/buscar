
#include "editDistOracle.h"


CheckInOracle( 
  EditDistOracle,
  "edit",
  "Edit distance\n\
  -f <file>         File with prototypes\n\
  [-w <i> <d> <s>]  Weights of insertion, deletion and substitution\n\
  Notes:\n\
  o The file of prototypes is composed of series of strings\n\
    separated by white spaces, tabs, newlines, etc.\n\
  o The poins are extracted randomly from the file."
)




//---------------------------------------------------------------------
//
EditDistOracle::EditDistOracle( string data ) 
{
	mWi = 1.0;
	mWd = 1.0;
	mWs = 2.0;

        string token;
        istringstream ss(data);
        while( ss >> token ) {
 
    		if( token == "-f" )
		{
			if( !(ss >> mFileProt) ) 
			{
				cerr << "ERROR in EditDistOracle: no file name" << endl;
				exit(-1);
			}
		}
		else if( token == "-w" ) {
			if( !(ss >> mWi) ) 
			{
				cerr << "ERROR in EditDistOracle: no insertion weight" << endl;
				exit(-1);
			}
			if( !(ss >> mWd) ) 
			{
				cerr << "ERROR in EditDistOracle: no deletion weight" << endl;
				exit(-1);
			}
			if( !(ss >> mWs) ) 
			{
				cerr << "ERROR in EditDistOracle: no substitution weight" << endl;
				exit(-1);
			}
		}
    		else 
    		{
			cerr << "ERROR in EditDistOracle: unrecognized option '" << token << "'" << endl;
			exit(-1);
		}
	}
	
	
	if( mFileProt.empty() )
	{
		cerr << "ERROR in EditDistOracle: no file name" << endl;
		exit(-1);
	}


	ReadPrototypes( mFileProt );

}


//---------------------------------------------------------------------
//
EditDistOracle::~EditDistOracle()
{
	delete [] mUsed;
}



//---------------------------------------------------------------------
//
void EditDistOracle::ReadPrototypes( string file )
{
	string cad;
	
	ifstream fProt( file.c_str() );
	if( !fProt ) {
		cerr << "ERROR ( EditDistOracle ): The file '" << file << "' can not be opened." << endl;
		exit(-1);
	}
	
	while( fProt >> cad )
		mTable.push_back(cad);
		
	fProt.close();
	
	if( mTable.size() == 0 ) {
    	cerr << "ERROR ( EditDistOracle ): The file '" << file << "' is empty." << endl;
    	exit(-1);
  	}	
	
	mUsed = new bool[ mTable.size() ];	
	for( unsigned int i = 0; i < mTable.size(); i++ )
		mUsed[i] = false;
}


//---------------------------------------------------------------------
//
Point EditDistOracle::NewPoint()
{
	if( mTable.size() == mPrototypes.size() ) 
	{   
		cerr << "ERROR ( EditDistOracle ): Not enough prototypes in the file '" << mFileProt << "'" << endl;
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
void EditDistOracle::PrintPoint( Point p )
{
	assert( p >= 0 && (unsigned)p < mPrototypes.size() );
	assert( (unsigned)mPrototypes[ p ] < mTable.size() );
	
	cout << mTable[ mPrototypes[ p ] ] << endl;
}




//---------------------------------------------------------------------
//
double EditDistOracle::CalculateDistance( Point p1, Point p2 )
{
	assert( p1 >= 0 && (unsigned)p1 < mPrototypes.size() );
	assert( (unsigned)mPrototypes[ p1 ] < mTable.size() );
	assert( p2 >= 0 && (unsigned)p2 < mPrototypes.size() );
	assert( (unsigned)mPrototypes[ p2 ] < mTable.size() );
	

	string cad1 = mTable[ mPrototypes[ p1 ] ];
	string cad2 = mTable[ mPrototypes[ p2 ] ];

	int lCad1 = cad1.size();
	int lCad2 = cad2.size();

	double D[lCad1+1][lCad2+1];

	D[0][0] = 0.0;
	
	for( int i = 1; i <= lCad1; i++ )
		D[i][0] = D[i-1][0] + mWd;

	for( int j = 1; j <= lCad2; j++ ) 
	{
		D[0][j] = D[0][j-1] + mWi;

		for( int i = 1; i <= lCad1; i++ ) 
		{
			double aux;

			if( cad1[i] == cad2[j] )
				D[i][j] = D[i-1][j-1];
			else 
				D[i][j] = D[i-1][j-1] + mWs;

			aux = D[i-1][j] + mWd;
		
			if( aux < D[i][j] )
				D[i][j] = aux;

			aux = D[i][j-1] + mWi;

			if( aux < D[i][j] )
				D[i][j] = aux;
		}
	}

  	return D[lCad1][lCad2];
	// de esta forma no se cumple la desigualdad triangular ...
	//  return D[lCad1][lCad2] / ( lCad1 + lCad2); 
}


