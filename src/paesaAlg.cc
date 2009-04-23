
#include "paesaAlg.h"


CheckInNNAlg(
  Paesa,
  "paesa",
  "P-Approximating and Eliminating Search Algorithm\n\
  [-r <int>]       gRep"
)


//-------------------------------------------------------------------
//
Paesa::Paesa( string data, Oracle * oracle )
{
	mOracle = oracle;	
	mNp = 0;
	//mNum_used_pivots = 0;
	//mGpaso_mas_cercano = 0;
	mGRep = 1;
	mTDist = NULL;
	mPivot = NULL;

        string token;
        istringstream ss(data);
        while( ss >> token ) {
		if( token == "-r" ) {
			if( !(ss >> mGRep) )
                        {
                                cerr << "ERROR in EditDistOracle: no insertion weight" << endl;
                                exit(-1);
                        }
		}
		else
               {
                        cerr << "ERROR in Paesa: unrecognized option '" << token << "'" << endl;
                        exit(-1);
                }
	}
	
}



//-------------------------------------------------------------------
//
Paesa::~Paesa() 
{
	if( mTDist != NULL ) 
	{
		for( int i = 0; i < mNp; i++ )
			delete[] mTDist[i];
		delete[] mTDist;
		delete[] mPivot;
	}
}




//-------------------------------------------------------------------
//
void Paesa::Insert( Point p )
{
	Point aux[] = {p}; 

	InsertBulk( aux, 1 );
}
	



//-------------------------------------------------------------------
//
void Paesa::InsertBulk( Point p[], int size )
{
		
	assert( size >= 0 );
	for( int i = 0; i < size; i++ )
  		assert( p[i] == mNp + i );
	
	
	if( mTDist != NULL ) {
		for( int i = 0; i < mNp; i++ )
			delete[] mTDist[i];
		delete[] mTDist;
		delete[] mPivot;
	}

	mNp += size;
	//np = ora->numProt();
    
	mTDist = new double*[ mNp ];
	for( int i = 0; i < mNp; i++ ) 
	{
		mTDist[i] = new double[ mNp ];
		
		for( int j = 0; j < mNp; j++ )
			mTDist[i][j] = mOracle->GetDistance(i, j);
	}
/*
	mPivot = new int[np];
	bool eliminado[np];
	double dis_pivots[np];
	for( int i = 0; i < np; i++) {
		eliminado[i] = false;
		dis_pivots[i] = 0.0;
	}
  
	int piv = 0;

	for( int k = 0; k < mNp; k++ ) {

		mPivot[k] = piv;
		eliminado[k] = true;

		// Actualización de la separacion a los piv
		for( int i = 0; i < np; i++ )  
			if( !eliminado[i] )
				dis_pivots[i] += tdist[piv][i];

		// Esoger el mejor pivot
		double max = 0.0;
		for( int i = 0; i < np; i++ ) 
			if( !eliminado[i] && dis_pivots[i] > max ) {
				max = dis_pivots[i];
				piv = i;
			}
    
	}
*/

	mPivot = new int[ mNp ];
	bool removed[ mNp ];
	double min_dis_pivots[ mNp ];
	
	for( int i = 0; i < mNp; i++) 
	{
		removed[i] = false;
		min_dis_pivots[i] = std::numeric_limits<double>::max();
	}

	int piv = 0;

	for( int k = 0; k < mNp; k++ ) 
	{
		mPivot[k] = piv;
		removed[k] = true;

 		// Actualización de la separacion a los piv
		for( int i = 0; i < mNp; i++ )
			if( !removed[i] && min_dis_pivots[i] > mTDist[piv][i] )
				min_dis_pivots[i] = mTDist[piv][i];

		// Esoger el mejor pivot
		double max = 0.0;
		for( int i = 0; i < mNp; i++ ) 
			if( !removed[i] && min_dis_pivots[i] > max ) {
				max = min_dis_pivots[i];
				piv = i;
			}
    
	}

	for( int i = 0; i < mNp; i++ )
		if( mPivot[i] < 0 || mPivot[i] >= mNp )
			cerr << "ERROR: incorrect pivot" << endl;

//	num_used_pivots = 0;
//	gpaso_mas_cercano = 0;
	
/*
	int* pivot2 = new int[mNp];
	int j = 0;
	for( int i = 1; i < mNp; i += 2 )
		pivot2[j++] = mPivot[i];
	for( int i = 0; i < mNp; i += 2 )
		pivot2[j++] = pivot[i];
  
	mPivot = pivot2;
	delete[] pivot2;
	
*/

}




//-------------------------------------------------------------------
// Point mNNPoint;		// masCercano
// double mNNDistance;	// disMin
void Paesa::SearchNN( Point p )
{
	bool *removed = new bool[mNp];
	double *G = new double[mNp];
	for( int i = 0; i < mNp; i++ ) {
		removed[i] = false;
		G[i] = 0;
	}

//	int paso = 0;
//	int paso_mas_cercano = 0;

	mNNPoint = -1;
	mNNDistance = std::numeric_limits<double>::max();

	double min_G = 0;
	double min_G_ant = 0.0;
	int piv = -1;

	int cont = 0;
	for( int j = 0; j < mNp; j++) 
	{
//		paso++;

//		num_used_pivots++;
		piv = mPivot[j];

		double dis_piv_mues = mOracle->GetDistance(p, piv);
		
		if( dis_piv_mues < mNNDistance ) 
		{
			mNNDistance = dis_piv_mues;
			mNNPoint = piv;
//			paso_mas_cercano = paso;
		}
		removed[piv] = true;

		for( int i = 0; i < mNp; i++ ) // Actualizar cota 
		{
			double dis_cir = fabs( mTDist[piv][i] - dis_piv_mues );
			
			if( G[i] < dis_cir )
				G[i] = dis_cir;
		}

		min_G_ant = min_G;
		min_G = std::numeric_limits<double>::max();
		piv = -1;
	
		for( int i = 0; i < mNp; i++ )  // Aproximación
		{
			if( !removed[i] && G[i] < min_G ) {
				min_G = G[i];
				piv = i;
			}
		}
		if( min_G <= min_G_ant ) {
			cont++;
		} else {
			cont = 0;
		}

		if( cont == mGRep ) 
			break;
			
	}//end for


	bool quedan_no_eliminados = true;
	
	while( quedan_no_eliminados ) 
	{
//		paso++;

		double dis_piv_mues = mOracle->GetDistance(p, piv);
		
		if( dis_piv_mues < mNNDistance ) {
			mNNDistance = dis_piv_mues;
			mNNPoint = piv;
//			paso_mas_cercano = paso;
		}
		removed[piv] = true;

		for( int i = 0; i < mNp; i++ ) // Actualizar cota
		{
			double dis_cir = fabs( mTDist[piv][i] - dis_piv_mues );
			if( G[i] < dis_cir )
				G[i] = dis_cir;
		}

		for( int i = 0; i < mNp; i++ )  // Eliminación
		{
			if( G[i] >= mNNDistance ) 
				removed[i] = true;
		}
     
		double min = std::numeric_limits<double>::max();
		piv = -1;
		for( int i = 0; i < mNp; i++ )  // Aproximación
		{
			if( !removed[i]) 
			{
				double aux =  G[i];
				if( aux < min ) {
					min = aux ;
					piv = i;
				}
			}
		}

		if( piv == -1 )
			quedan_no_eliminados = false;

	}
	
//	gpaso_mas_cercano += paso_mas_cercano;
  
  
	delete [] removed;	
	delete [] G;
}


