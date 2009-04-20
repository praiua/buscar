

#include "aesaAlg.h"


CheckInNNAlg(
  Aesa,
  "aesa",
  "Approximating and Eliminating Search Algorithm\n\
  No options"
)


//-------------------------------------------------------------------
//
Aesa::Aesa( vector<string> data, Oracle * oracle )
{
	mOracle = oracle;
		
	if( data.size() != 1 )
	{
		cerr << "ERROR (" << data[0] << "): Wrong input parameters" << endl;
		cerr << "  Unknown '" << VectorToString( data ) << "' options" << endl;
		cout << "Usage: " << endl;
		CheckInNNAlg::ListInfo( data[0] );	
		exit(-1);
	}
}


//-------------------------------------------------------------------
//
void Aesa::Insert( Point p )
{
	assert( p >= 0 && (unsigned)p == mTDist.size() );

	vector<double> v;
	
	for( int i = 0; i < p+1; i++ )
	{
		v.push_back( mOracle->GetDistance( i, p ) );
	}
		
	mTDist.push_back( v );

	for( int i = 0; i < p+1; i++ ) 
	{		
		mTDist[i].push_back( mTDist[p][i] ); 
	}
	
	
/*	for( unsigned int i = 0; i < mTDist.size(); i++ ) 
	{
		for( unsigned int j = 0; j < mTDist.size(); j++ ) 
		{
			printf("%.2f ", mTDist[i][j]);
		}
		cout << endl;
	}*/
}
	



//-------------------------------------------------------------------
//
void Aesa::InsertBulk( Point p[], int size )
{
	for(int i=0; i<size; i++)
		Insert( p[i] );
}




//-------------------------------------------------------------------
//
//double &disMin, // mNNDistance
//int &masCercano // mNNPoint)

void Aesa::SearchNN( Point p ) 
{
	mNNPoint = -1;	
	mNNDistance = std::numeric_limits<double>::max();
//	gpaso_mas_cercano = 0;
	
	int np = mTDist.size();
	bool *removed = new bool[ np ];
	double *G = new double[ np ];
	
	for( int i = 0; i < np; i++ ) {
		removed[i] = false;
		G[i] = 0;
	}

	int paso = 0;
	//int paso_mas_cercano = 0;
	int piv = 0;
	double dis_piv_mues = 0;
	bool quedan_no_eliminados = true;	
  
  
	while( quedan_no_eliminados ) 
	{
		paso++;

		dis_piv_mues = mOracle->GetDistance( p, piv );
    
		if( dis_piv_mues < mNNDistance ) {
			mNNDistance = dis_piv_mues;
			mNNPoint = piv;
      		//paso_mas_cercano = paso;
    	}
    	removed[piv] = true;

    	for( int i = 0; i < np; i++ )  // Actualizar cota
    	{
      		double dis_cir = fabs( mTDist[piv][i] - dis_piv_mues );
      		
      		if( G[i] < dis_cir )
        		G[i] = dis_cir;
    	}


		for( int i = 0; i < np; i++ )  // Eliminación
    	{
      		if( G[i] >= mNNDistance ) 
        		removed[i] = true;
    	}
     
		
		double min_G = std::numeric_limits<double>::max();
    	piv = -1;
    
		for( int i = 0; i < np; i++ )  // Aproximación
		{
      		if( !removed[i] && G[i] < min_G ) 
      		{
				min_G = G[i];
				piv = i;
			}
		}
    
		if( piv == -1 )
			quedan_no_eliminados = false;

	}//end while
  
	//gpaso_mas_cercano += paso_mas_cercano;
  
  
	delete [] removed;
	delete [] G;
}


