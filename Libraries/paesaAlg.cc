
#include "paesaAlg.h"


CheckInNNAlg(
  Paesa,
  "paesa",
  "P - Approximating and Eliminating Search Algorithm\n\
  -r <int>       gRep"
)


//-------------------------------------------------------------------
//
Paesa::Paesa( vector<string> data, Oracle * oracle )
{
	mOracle = oracle;	
	mNp = -1;
	//mNum_used_pivots = 0;
	//mGpaso_mas_cercano = 0;
	mGRep = 1;
	mTDist = 0;
	mPivot = 0;


	for(unsigned int i = 1; i < data.size(); i++ )
	{
    	if( data[i] == "-r" && i < data.size() - 1 &&
			StringToInt(data[++i], gRep) && gRep < 0)
		{
			;
		}
    	else 
    	{
    		cerr << "ERROR (" << data[0] << "): Wrong input parameters" << endl;
			cerr << "  Unknown '" << VectorToString( data ) << "' options" << endl;
			cout << "Usage: " << endl;
			CheckInNNAlg::ListInfo( data[0] );
			exit(-1);
		}
	}
	
}



//-------------------------------------------------------------------
//
Paesa::~Paesa() 
{
	//int *pivot;          // Pivotes
	//double **tdist;      // tabla de distancias
}




//-------------------------------------------------------------------
//
void Paesa::Insert( Point p )
{
/*  	assert( p >= 0 && p == mNum_p );
  	assert (mAlg_pb == "minmax" || mAlg_pb == "minsup");
  	
	Point aux[] = {p}; 

	InsertBulk( aux, 1 );*/
}
	



//-------------------------------------------------------------------
//
void Paesa::InsertBulk( Point p[], int size )
{
	
	if( tdist != 0 ) {
		for( int i = 0; i < np; i++ )
			delete[] tdist[i];
		delete[] tdist;
		delete[] pivot;
	}


	np = ora->numProt();
    
	tdist = new double*[np];
	for( int i = 0; i < np; i++ ) 
	{
		tdist[i] = new double[np];
		for( int j = 0; j < np; j++ )
			tdist[i][j] = ora->distancia(i,j);
	}
/*
	pivot = new int[np];
	bool eliminado[np];
	double dis_pivots[np];
	for( int i = 0; i < np; i++) {
		eliminado[i] = false;
		dis_pivots[i] = 0.0;
	}
  
	int piv = 0;

	for( int k = 0; k < np; k++ ) {

		pivot[k] = piv;
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

	pivot = new int[np];
	bool eliminado[np];
	double min_dis_pivots[np];
	for( int i = 0; i < np; i++) 
	{
		eliminado[i] = false;
		min_dis_pivots[i] = std::numeric_limits<double>::max();
	}

	int piv = 0;

	for( int k = 0; k < np; k++ ) 
	{
		pivot[k] = piv;
		eliminado[k] = true;

 		// Actualización de la separacion a los piv
		for( int i = 0; i < np; i++ )
			if( !eliminado[i] && min_dis_pivots[i] > tdist[piv][i] )
				min_dis_pivots[i] = tdist[piv][i];

		// Esoger el mejor pivot
		double max = 0.0;
		for( int i = 0; i < np; i++ ) 
			if( !eliminado[i] && min_dis_pivots[i] > max ) {
				max = min_dis_pivots[i];
				piv = i;
			}
    
	}

	for( int i = 0; i < np; i++ )
		if( pivot[i] < 0 || pivot[i] >= np )
			cerr << "ERROR: incorrect pivot" << endl;

//	num_used_pivots = 0;
//	gpaso_mas_cercano = 0;
	
/*
	int* pivot2 = new int[np];
	int j = 0;
	for( int i = 1; i < np; i += 2 )
		pivot2[j++] = pivot[i];
	for( int i = 0; i < np; i += 2 )
		pivot2[j++] = pivot[i];
  
	delete[] pivot;
	pivot = pivot2;
*/

}




//-------------------------------------------------------------------
// Point mNNPoint;		// masCercano
// double mNNDistance;	// disMin
void Paesa::SearchNN( Point p )
{
	bool *removed = new bool[np];
	double *G = new double[np];
	for( int i = 0; i < np; i++ ) {
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
	for( int j = 0; j < np; j++) 
	{
//		paso++;

//		num_used_pivots++;
		piv = pivot[j];

		double dis_piv_mues = mOracle->GetDistance(p, piv);
		
		if( dis_piv_mues < mNNDistance ) 
		{
			mNNDistance = dis_piv_mues;
			mNNPoint = piv;
//			paso_mas_cercano = paso;
		}
		removed[piv] = true;

		for( int i = 0; i < np; i++ ) // Actualizar cota 
		{
			double dis_cir = fabs( tdist[piv][i] - dis_piv_mues );
			
			if( G[i] < dis_cir )
				G[i] = dis_cir;
		}

		min_G_ant = min_G;
		min_G = std::numeric_limits<double>::max();
		piv = -1;
	
		for( int i = 0; i < np; i++ )  // Aproximación
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

		if( cont == gRep ) 
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

		for( int i = 0; i < np; i++ ) // Actualizar cota
		{
			double dis_cir = fabs( tdist[piv][i] - dis_piv_mues );
			if( G[i] < dis_cir )
				G[i] = dis_cir;
		}

		for( int i = 0; i < np; i++ )  // Eliminación
		{
			if( G[i] >= mNNDistance ) 
				removed[i] = true;
		}
     
		double min = std::numeric_limits<double>::max();
		piv = -1;
		for( int i = 0; i < np; i++ )  // Aproximación
		{
			if( !eliminado[i]) 
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
	double [] G;
}


