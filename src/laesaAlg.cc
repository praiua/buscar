#include <map>
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
//	mNum_p = 0;
//	mNum_pnb = 0;
	mMaxBp = 2;
	 mAlgBp = "minmax";

        string token;
        istringstream ss(data);
        while( ss >> token ) 
	{
    		if( token == "-bpa" )
		{
                       if( !(ss >>  mAlgBp) )
                        {
                                cerr << "ERROR in Laesa: no file name" << endl;
                                exit(-1);
                        }
		}
		else if( token == "-bp" )
		{
                        if( !(ss >> mMaxBp) )
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

	if(  mAlgBp != "minmax" &&  mAlgBp != "minsup") 
	{
		cerr << "ERROR in Laesa: wrong algorithm '" <<  mAlgBp << "'" << endl;
                exit(-1);
 	} 

	if( mMaxBp < 0 )
	{
		cerr << "ERROR in Laesa: wrong number of bp (" << mMaxBp << ")" << endl;
                exit(-1);
	}
	
	
}


//-------------------------------------------------------------------
//
Laesa::~Laesa() 
{
}


//-------------------------------------------------------------------
//
void Laesa::Min_max() 
{
	// initilize the vector of minimum distances
	vector<double> minDis(mP.size(), numeric_limits<double>::max() );
	

	// the first base prototype is '0'
	int i = 0;
	mBp[i] = 0;	

	while(true)
	{  
		mIsBase[mBp[i]] = true;

		// complete the table of distances and update the vector 
		// of minimum distances

		for( unsigned int j = 0; j < mP.size(); j++ ) 
		{
			mBpDis[j][i] = mOracle->GetDistance( mP[mBp[i]], mP[j] );

			if( mBpDis[j][i] < minDis[j] )
				minDis[j] = mBpDis[j][i];
		}

		i++;
		if( i == mNumBp )
			break;
		
		// search the following base prototype
		double max = -1.0;
		for( unsigned int j = 0; j < mP.size(); j++ ) 
		{
			if( !mIsBase[j] && minDis[j] > max ) 
			{
				max = minDis[j];
				mBp[i] = j;
			}
		}

		if( max == -1.0 ) {
			cerr << "ERROR (Laesa)::Min_max() something wrong is happening." << endl;
			exit(-1);
		}

	}
}



//-------------------------------------------------------------------
//
void Laesa::InsertBulk( Point p[], int size )
{
	assert( size >= 0 );
  	assert ( mAlgBp == "minmax" ||  mAlgBp == "minsup");
  	
	// I clear everything
	mP.clear();
	mBp.clear();
	mIsBase.clear();
    	mBpDis.clear();

	// Updating the list of prototypes
	mP = vector<Point>(p, p + size);
	mNumBp = min( (int)mP.size(), mMaxBp );
        mBp = vector<int>( mNumBp, -1 );
	mIsBase = vector<bool>( size, false );
//	mBpDis = vector< vector<double> >( size, vector<double>( mMaxBp, numeric_limits<double>::max() ));
	vector<double> aux( mMaxBp, numeric_limits<double>::max() );
        for( int i = 0; i < size; i++ )
		mBpDis.push_back(aux);

	Min_max(); // !!!! por ahora solo "minmax" !!!!
		
}

//-------------------------------------------------------------------
//
/*
void Laesa::Insert( Point p )
{
	Point aux[] = {p}; 
	InsertBulk( aux, 1 );
}
*/	

void Laesa::Insert( Point p )   // !!!! Only Min_max !!!!
{

	mP.push_back(p);
	int npi = mP.size() - 1;

	mNumBp = min( (int)mP.size(), mMaxBp );
        mBp = vector<int>( mNumBp, -1 );
	mIsBase = vector<bool>( mP.size(), false );

	mBpDis.push_back( vector<double>(mMaxBp, numeric_limits<double>::max()) );
        
	// initilize the vector of minimum distances
	vector<double> minDis(mP.size(), numeric_limits<double>::max() );
	

	// the first base prototype is '0'
	int i = 0;
	mBp[i] = 0;	

	bool dirty = false;
	while(true)
	{  
		mIsBase[mBp[i]] = true;

		// complete the table of distances and update the vector 
		// of minimum distances

		for( unsigned int j = 0; j < mP.size(); j++ ) 
		{
			if( (int) j == npi || dirty) 
				mBpDis[j][i] = mOracle->GetDistance( mP[mBp[i]], mP[j] );

			if( mBpDis[j][i] < minDis[j] )
				minDis[j] = mBpDis[j][i];
		}

		i++;
		if( i == mNumBp )
			break;
		
		// search the following base prototype
		double max = -1.0;
		for( unsigned int j = 0; j < mP.size(); j++ ) 
		{
			if( !mIsBase[j] && minDis[j] > max ) 
			{
				max = minDis[j];
				mBp[i] = j;
			}
		}

		if( max == -1.0 ) {
			cerr << "ERROR (Laesa)::Min_max() something wrong is happening." << endl;
			exit(-1);
		}

		if( mBp[i] == npi )
			dirty = true;

	}

}



//-------------------------------------------------------------------
//
/*    YA LO MIRARE 
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



	for( int b = 0; b < mMaxBp; b++ ) 
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
		
		mBp[b] = i_cont_min;
		
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

	for( int i = 0; i < mMaxBp; i++ )
		for( int j = 0; j < mNum_p; j++ )
			mBpDis[i][j] = dis[ mBp[i] ][j];

	for( int i = 0; i < mNum_p; i++ )
		mEs_base[i] = false;
	
	for( int i = 0; i < mMaxBp; i++ ) {
//    cout << pb[i] << endl;
		mEs_base[ mBp[i] ] = true;
  	}
}
*/


//-------------------------------------------------------------------
//
void Laesa::SearchNN( Point p ) 
{
	assert( p >= 0 );
	
	mNNDistance = std::numeric_limits<double>::max();
	mNNPoint = -1;
	
	vector<double> bpToXDis( mNumBp );

	// compute the distance to every base prototype
	for( int i = 0; i < mNumBp ; i++ ) 
	{
		double dis = mOracle->GetDistance( p, mP[mBp[i]] );
		
		if( dis < mNNDistance ) 
		{
			mNNDistance = dis;
			mNNPoint = mP[mBp[i]];
		}
		bpToXDis[i] = dis;
	}

	// updating the lower bound of the distance (g)

	multimap<double,int> elem; // I'am using it as a priority queue

	for( unsigned int j = 0; j < mP.size(); j++ ) 
	{
		if( !mIsBase[j] ) 
		{
			double g = 0.0;
			for( int i = 0; i < mNumBp; i++ ) 
			{
				double dif = fabs( bpToXDis[i] - mBpDis[j][i]);
				
				if( dif > g )
					g = dif;
			}
			elem.insert( make_pair(g, j) );
		}
	}


	for( multimap<double,int>::iterator i = elem.begin(); i != elem.end(); ++i ) 
	{    
		if( i->first > mNNDistance )
			break;

		double dis = mOracle->GetDistance( p, mP[i->second] );
		
		if( dis < mNNDistance ) 
		{
			mNNDistance = dis;
			mNNPoint = mP[ i->second];
    		}

	}

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
