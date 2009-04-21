
#include "laesaAlg.h"


CheckInNNAlg(
  Laesa,
  "laesa",
  "Linear Approximating and Eliminating Search Algorithm\n\
  [-bp  <int>]    Number of base prototypes.\n\
  [-bpa <alg>]    Algorithm for the selection of base prototypes:\n\
                  minmax, minsup"
)


//-------------------------------------------------------------------
//
Laesa::Laesa( string data, Oracle * oracle ) 
{
	mOracle = oracle;
	mNum_p = 0;
	mNum_pnb = 0;
	mNum_pb = 2;
	mAlg_pb = "minmax";

        string token;
        istringstream ss(data);
        while( ss >> token ) 
	{
    		if( token == "-bpa" )
		{
                       if( !(ss >> mAlg_pb) )
                        {
                                cerr << "ERROR in Laesa: no file name" << endl;
                                exit(-1);
                        }
		}
		else if( token == "-bp" )
		{
                        if( !(ss >> mNum_pb) )
                        {
                                cerr << "ERROR in Laesa: no base prototypes given" << endl;
                                exit(-1);
                        }
		}
                else
                {
                        cerr << "ERROR in EditDistOracle: unrecognized option '" << token << "'" << endl;
                        exit(-1);
                }
	}

	if( mAlg_pb != "minmax" && mAlg_pb != "minsup") 
	{
		cerr << "ERROR in Laesa: wrong algorithm '" << mAlg_pb << "'" << endl;
                exit(-1);
 	} 

	if( mNum_pb < 0 )
	{
		cerr << "ERROR in Laesa: wrong number of bp (" << mNum_pb << ")" << endl;
                exit(-1);
	}
	
	
	mPb = new int[ mNum_pb ];
	mDis_pb = new double*[ mNum_pb ];
	mEs_base = NULL;
	
	for( int i = 0; i < mNum_pb; i++ )
		mDis_pb[i] = NULL;
	
}


//-------------------------------------------------------------------
//
Laesa::~Laesa() 
{
	delete [] mPb;
	
	if( mEs_base  != NULL )
		delete [] mEs_base;
	
	for( int i = 0; i < mNum_pb; i++ )
		delete [] mDis_pb[i];
	delete [] mDis_pb;	
}

//-------------------------------------------------------------------
//
void Laesa::Insert( Point p )
{
	Point aux[] = {p}; 

	InsertBulk( aux, 1 );
}
	



//-------------------------------------------------------------------
//
void Laesa::InsertBulk( Point p[], int size )
{
	assert( size >= 0 );
  	assert (mAlg_pb == "minmax" || mAlg_pb == "minsup");
  	for( int i = 0; i < size; i++ )
  		assert( p[i] == mNum_p + i );
  	
  	mNum_p += size;

	// reservo espacio para la tabla de distancias

  	for( int i = 0; i < mNum_pb; i++ ) 
  	{
    	delete [] mDis_pb[i];    		
    	mDis_pb[i] = new double[ mNum_p ];
  	}

	delete [] mEs_base;
	mEs_base = new bool[ mNum_p ];
	

	if( mAlg_pb == "minmax" )
		Min_max( );
		
	else if( mAlg_pb == "minsup" )
		Min_sup( );
		

	mNum_pnb = mNum_p - mNum_pb;
//  cout << "num_pb :" << num_pb << endl;
//  cout << "num_pnb:" << num_pnb << endl;
}




//-------------------------------------------------------------------
//
void Laesa::Min_sup() 
{
	double dis[mNum_p][mNum_p];
	double dmc[mNum_p];
	int vivos[mNum_p][mNum_p];
	int n_vivos[mNum_p];	

	for( int i = 0; i < mNum_p; i++ )
		for( int j = 0; j < mNum_p; j++ )
			dis[i][j] = mOracle->GetDistance(i, j);	
	
	for( int i = 0; i < mNum_p; i++ ) 
	{
		dmc[i] = std::numeric_limits<double>::max();
		
		for( int j = 0; j < mNum_p; j++ ) 
		{
			if( j != i ) 
			{
				if( dis[i][j] < dmc[i] )
	  				dmc[i] = dis[i][j];
      		}
    	}
  	}
  	  	
  	
	for( int i = 0; i < mNum_p; i++ )
		for( int j = 0; j < mNum_p; j++ )
			vivos[i][j] = j;
			
	
	for( int i = 0; i < mNum_p; i++ )
		n_vivos[i] = mNum_p;



	for( int b = 0; b < mNum_pb; b++ ) 
	{
		int cont_min = std::numeric_limits<int>::max();
		int i_cont_min = -1;
		
		for( int i = 0; i < mNum_p; i++ ) 
		{			
			int cont = 0;

			for( int j = 0; j < mNum_p; j++ ) 
			{
				for( int k = 0; k < n_vivos[j]; k++ ) 
				{
					if( fabs(dis[i][j] - dis[i][vivos[j][k]]) <= dmc[j] ) 
						cont++;
				}
			}

			if( cont < cont_min ) 
			{
				cont_min = cont;
				i_cont_min = i;
			}
		}
		
		mPb[b] = i_cont_min;
		
		for( int j = 0; j < mNum_p; j++ ) 
		{
			int cont = 0;
			for( int k = 0; k < n_vivos[j]; k++ ) 
			{
				if( fabs( dis[i_cont_min][j] - dis[i_cont_min][vivos[j][k]])<=dmc[j] )
				{
					vivos[j][cont] = vivos[j][k];
					cont++;
				}
			}
			n_vivos[j] = cont;
		}
    
//    cout << cont_min / double(n_prot) << endl;
	}

	for( int i = 0; i < mNum_pb; i++ )
		for( int j = 0; j < mNum_p; j++ )
			mDis_pb[i][j] = dis[ mPb[i] ][j];

	for( int i = 0; i < mNum_p; i++ )
		mEs_base[i] = false;
	
	for( int i = 0; i < mNum_pb; i++ ) {
//    cout << pb[i] << endl;
		mEs_base[ mPb[i] ] = true;
  	}
}


//-------------------------------------------------------------------
//
void Laesa::Min_max() 
{
	// inicio el vector de distancias mnimas
	double *d_min = new double[ mNum_p ];	
	
	for( int i = 0; i < mNum_p; i++ ) {
		d_min[i] = std::numeric_limits<double>::max();
		mEs_base[i] = false;
	}

	int i = 0;	// el primer prototipo base es el '0'
	mPb[0] = 0;	
	mEs_base[0] = true;

	while( true ) 
	{  
		// relleno la tabla de distancias y acutalizo el vector 
		// de distancias minimas

		for( int j = 0; j < mNum_p; j++ ) 
		{
			mDis_pb[i][j] = mOracle->GetDistance( mPb[i], j );

			if( mDis_pb[i][j] < d_min[j] )
				d_min[j] = mDis_pb[i][j];
		}
		
		i++;
		if( i == mNum_pb )
			break;

		// busco el siguiente prototipo base
		double max = -1.0;
		for( int j = 0; j < mNum_p; j++ ) 
		{
			if( !mEs_base[j] && d_min[j] > max ) 
			{
				max = d_min[j];
				mPb[i] = j;
			}
		}

		if( mNum_p > 1 && max == -1.0 ) {
			cerr << "ERROR (Laesa)::Min_max() function: Not enough prototypes." << endl;
			exit(-1);
		}

		mEs_base[ mPb[i] ] = true;
	}
	
	
	delete d_min;
}



//-------------------------------------------------------------------
//
struct Elem {
  double g;
  int    ind;
};

static int CompLaesa( const void* elem1, const void* elem2 ) 
{
	if( ((Elem*)elem1)->g < ((Elem*)elem2)->g )
		return -1;
		
	if( ((Elem*)elem1)->g > ((Elem*)elem2)->g )
		return 1;
		
	return 0;  
}


//-------------------------------------------------------------------
//
void Laesa::SearchNN( Point p ) 
{
	assert( p >= 0 );
	
	mNNDistance = std::numeric_limits<double>::max();
	mNNPoint = -1;
	
	double *dis_pb_x = new double[ mNum_pb ];

	// calculo distancias a los prototipos base
	for( int i = 0; i < mNum_pb; i++ ) 
	{
		double dis = mOracle->GetDistance( p, mPb[i] );
		
		if( dis < mNNDistance ) 
		{
			mNNDistance = dis;
			mNNPoint = mPb[i];
		}
		dis_pb_x[i] = dis;
	}

	// actualizo las cotas inferiores (g)
  	Elem *elem = new Elem[ mNum_pnb ];

	int cont = 0;
	for( int j = 0; j < mNum_p; j++ ) 
	{
		if( !mEs_base[j] ) 
		{
			elem[cont].g = 0.0;
			elem[cont].ind = j;
			
			for( int i = 0; i < mNum_pb; i++ ) 
			{
				double dif = fabs( dis_pb_x[i] - mDis_pb[i][j]);
				
				if( dif > elem[cont].g )
					elem[cont].g = dif;
			}
			cont++;
		}
	}

	if( cont != mNum_pnb ) {
		cerr << "ERROR (Laesa)::SearchNN() function:" << endl;
		cerr << "  'cont != num_pnb' " << endl;
		cerr << "   cont   : " << cont << endl;
		cerr << "   num_pnb: " << mNum_pnb << endl;
		exit(-1);
	}
    

	qsort( elem, mNum_pnb, sizeof(Elem), CompLaesa );

	for( int i = 0; i < mNum_pnb; i++ ) 
	{    
		if( elem[i].g > mNNDistance )
			break;

		// calculo la distancia
		double dis = mOracle->GetDistance( p, elem[i].ind );
		
		if( dis < mNNDistance ) 
		{
			mNNDistance = dis;
			mNNPoint = elem[i].ind;
    	}

	}

	delete [] elem;
	delete [] dis_pb_x;
}


/*
//-------------------------------------------------------------------
//
void Laesa::SearchNN( Point p ) 
{
  mNNDistance = std::numeric_limits<double>::max();;
  mNNPoint = -1;

  int  num_p = ora->numProt();
  double g[num_p];
  bool usado[num_p];

  for( int i = 0; i < num_p; i++ ) {
    g[i] = 0.0;
    usado[i] = false;
  }

// calculo distancias a los prototipos base

  for( int i = 0; i < num_pb; i++ ) {
    double dis = ora->distancia(pb[i]);
    if( dis < mNNDistance ) {
      mNNDistance = dis;
      mNNPoint = pb[i];
    }
    usado[pb[i]] = true;
    g[pb[i]] = dis;
  }

// actualizo las cotas inferiores (g)

  for( int j = 0; j < num_p; j++ ) {
    if( !usado[j] ) {
      for( int i = 0; i < num_pb; i++ ) {
	double dif = fabs( g[pb[i]] - dis_pb[i][j]);
	if( dif > mNNDistance ) {  // elimino
	  usado[j] = true;
	  break;
	}
	if( dif > g[j] )
	  g[j] = dif;
      }
    }
  }

  while( true ) {
    
// busco un candidato a mas cercano
    
    double g_min = std::numeric_limits<double>::max();
    int ind_g_min = -1;

    for( int i = 0; i < num_p; i++ )
      if( !usado[i] && g[i] < g_min ) {
        g_min = g[i];
	ind_g_min = i;
      }

// si no lo encuentro, termino.

    if( ind_g_min == -1 )
      break;

// calculo la distancia

    double dis = ora->distancia(ind_g_min);
    if( dis < mNNDistance ) {
      mNNDistance = dis;
      mNNPoint = ind_g_min;
    }
    usado[ind_g_min] = true;

// elimino (solo si mNNDistance decrece, pero creo que se puede evitar)

    for( int i = 0; i < num_p; i++ )
      if( !usado[i] && g[i] > dis_min )
        usado[i] = true;

  }

}
*/
