
#include "vptreeAlg.h"


CheckInNNAlg(
  Vptree,
  "vptree",
  "Vantage Point Tree (vp-tree)\n\
  Organize groups according to their distance to a prototype.\n\
  [-nc <int>]             Number of children\n\
  [-r <1|2>]              Elimination rule\n\
  [-ext]                  Use the representatives at the extremes"
)


//-------------------------------------------------------------------
//
Vptree::Vptree( vector<string> data, Oracle *oracle ) 
{
	mOracle = oracle;
	mRule1 = false;
	mRule2 = false;
	mExtrem = false;
	mNumChild = 2;
	mRaiz = 0;	 
	bool error = false;

	for(unsigned int i = 1; i < data.size() && !error; i++ )
	{
    	if( data[i] == "-ext" ) 
    	{
    		mExtrem = true;
    	}
    	else if( data[i] == "-nc" && i < data.size() - 1 &&
			StringToInt(data[++i], mNumChild) && mNumChild > 0)
		{
			;
		}
    	else if( data[i] == "-r" && i < data.size() - 1)
		{
			i++;
			for( unsigned int j = 0; j < data[i].length(); j++ ) 
			{
		        if( data[i][j] == '1' )
					mRule1 = true;
				else if( data[i][j] == '2' ) 
					mRule2 = true;
				else {
					error = true;
				}
			}
		}		
    	else 
    	{
    		error = true;			
		}
	}	
	
	if( error )
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
Vptree::~Vptree()
{
	delete mRaiz;
}



//-------------------------------------------------------------------
//
void Vptree::Insert( Point p )
{
	Point aux[] = {p}; 

	InsertBulk( aux, 1 );
}
	


//-------------------------------------------------------------------
//
void Vptree::InsertBulk( Point p[], int size )
{
	assert( size >= 0 );
	
	for( int i = 0; i < size; i++ ) 
	{
		assert( (unsigned)p[i] == mData.size() );
		mData.push_back( p[i] );
	}
		
	
	if( mRaiz != 0 )
		delete mRaiz;
	

	mRaiz = BuildTree( mData );
	// Check( mRaiz, mData.size() );
	
	mVisited = 0;
}




//-------------------------------------------------------------------
// Point mNNPoint;		// masCercano
// double mNNDistance;	// disMin
void Vptree::SearchNN( Point p )
{
	mNNDistance = std::numeric_limits<double>::max();
	mNNPoint = -1;
	mLevel = 0;			
	Search( p, mRaiz, mNNDistance, mNNPoint );
}

//-------------------------------------------------------------------
//
void Vptree::Search( Point p, const Tree  *a, double &dis_min, int &mas_cercano) 
{
	if( a == 0 ) {
		cerr << "ERROR(Vptree): SearchNN(...): empty tree" << endl;
		exit(-1);
	}

	mLevel++;

	mVisited++;

	double dis = mOracle->GetDistance( p, a->mRep );
	if( dis < dis_min ) {
 		dis_min = dis;
		mas_cercano = a->mRep;
	}

	bool *usado = new bool[mNumChild];

	int cont = 0;
	for( int i = 0; i < a->mNumChild ; i++ )
		if( a->mChild[i] == 0 ) 
			usado[i] = true;
		else {
			usado[i] = false;
			cont++;
		}

	for( int i = 0; i < cont; i++ ) 
	{    
		double min_dis = std::numeric_limits<double>::max();
		int min_hijo = -1;

		for( int j = 0; j < mNumChild; j++ ) 
		{
			if( !usado[j] ) {
				double d;
				if( a->mRadio_int[j] < dis ) 
				{
					if( a->mRadio_ext[j] > dis )
						d = 0.0;
					else 
						d = dis - a->mRadio_ext[j];
				} 
				else {
					d = a->mRadio_int[j] - dis;
				}

				if( d < min_dis ) {
					min_dis = d;
					min_hijo = j;
				}
			}
		}


		if( min_hijo == -1 ) {
			cerr << "ERROR(Vptree): SearchNN(...): min_hijo == -1" << endl;
			exit(-1);
		}

		usado[min_hijo] = true;
		
		if( ( !mRule1 || dis - dis_min < a->mRadio_ext[min_hijo] ) &&
			( !mRule2 || dis + dis_min > a->mRadio_int[min_hijo] ) ) 
		{
/*
			cout << nivel << " rep: " << a->rep << " min_hijo: " << min_hijo
				<< " min_dis: " << min_dis << " dis: " << dis 
				<< " radio_ext: " << a->radio_ext[min_hijo]
				<< " radio_int: " << a->radio_int[min_hijo] << endl;
*/
			Search( p, a->mChild[min_hijo], dis_min, mas_cercano );
		}
	}
	
	mLevel--;
	
	delete[] usado;
}




//-------------------------------------------------------------------
//
void Vptree::Check( const Vptree::Tree *a, bool *used, int num_punt ) 
{
	if( a == 0 )
		return;

	if( a->mRep >= num_punt || a->mRep < 0 ) {
		cerr << "ERROR(Vptree): Check(...): representante fuera de rango" << endl;
		exit(-1);
	}

	if( used[ a->mRep ] ) {
		cerr << "ERROR(Vptree): Check(...): un representante aparece dos veces" << endl;
		exit(-1);
	}

	used[ a->mRep ] = true;

	for( int i = 0; i < a->mNumChild; i++ ) 
	{
		Check( a->mChild[i], used, num_punt );
	}
}


//-------------------------------------------------------------------
//
void Vptree::Check( Vptree::Tree *a, int num_punt ) 
{
	bool *used = new bool[num_punt];

	for( int i = 0; i < num_punt; i++ )
		used[i] = false;

	Check(a, used, num_punt);

	for( int i = 0; i < num_punt; i++ )
		if( !used[i] ) {
			cerr << "ERROR(Vptree): Check(...): representante no usado" << endl;
			exit(-1);
		}
		
	delete [] used;
}


//-------------------------------------------------------------------
//
struct sdis {
	int p;
	double dis;
};

static int Comp( const void *p1, const void *p2 ) 
{
	sdis *e1 = (sdis*) p1;
	sdis *e2 = (sdis*) p2;

	if( e1->dis < e2->dis )
		return -1;
	if( e1->dis > e2->dis )
		return 1;
	return 0;
}



//-------------------------------------------------------------------
//
Vptree::Tree* Vptree::BuildTree( vector<int> data ) 
{
	assert( data.size() >= 0);

	if( data.size() == 0 ) {
		return 0;
	}

	Tree *a = new Tree;
	a->mNumChild = mNumChild;
	a->mRadio_int = new double[ mNumChild ];
	a->mRadio_ext = new double[ mNumChild ];
	a->mChild = new Tree*[ mNumChild ];

	// Calcular el representante.
  
	// Si no se dice nada cojo el primero (el cero).

	double disMax = -1.0;
	a->mRep = data[0];

	if( mExtrem ) {
		// si me interesa que sea un extremal
		// cojer el mas lejano a un prototipo arbitrario (el 0)

		for( unsigned int i = 1; i < data.size(); i++ ) 
		{
			double dis = mOracle->GetDistance( data[0], data[i] );
			if( dis > disMax ) {
				disMax = dis;
				a->mRep = data[i];
			}
		}
	}

	// primero calculo la distancia de los prototipos al representante
	
	sdis *dist = new sdis[ data.size() ];
	int num_dis = 0;
	for( unsigned int i = 0; i < data.size(); i++ ) 
	{
		if( data[i] != a->mRep ) {
			dist[num_dis].dis = mOracle->GetDistance( data[i], a->mRep );
			dist[num_dis].p = data[i];
			num_dis++;
		}
	}

	// voy a escojer un radio de forma que la mitad de los prototipos
	// disten menos que el radio del representante

  
	qsort( dist, num_dis, sizeof(sdis), Comp );
	
	vector<int> p_bloque;
	int total = 0;
	for( int i = 0; i < mNumChild; i++ ) 
	{
		p_bloque.clear();
		//int p_bloque[ num_dis / mNumChild + 1 ];

		int primero = i * num_dis / mNumChild;
		int ultimo  = ( i + 1 ) * num_dis / mNumChild;

		if( primero == ultimo ) { // bloque vacio
			a->mRadio_int[i] = 0; // por poner algo ...
			a->mRadio_ext[i] = 0;
			a->mChild[i] = 0;  // esto es lo importante
			continue;
		}

		if( ultimo <= 0 ) {
			cerr << "ERROR(Vptree): ultimo: " << ultimo << endl;
			exit(-1);
		}

		a->mRadio_int[i] = dist[primero].dis;
		a->mRadio_ext[i] = dist[ultimo-1].dis;

		//int cont = 0;
		for( int j = primero; j < ultimo; j++ ) 
		{
			p_bloque.push_back( dist[j].p );
			//p_bloque[cont] = dist[j].p;
			//cont++;
		}

		//if(cont > num_dis / mNumChild + 1 ) {
		if( (signed)p_bloque.size() > num_dis / mNumChild + 1 ) {
			cerr << "ERROR(Vptree): cont = " << p_bloque.size()  
				<< " num_dis/num_hijos: " << num_dis / double(mNumChild)
				<< " primero: " << primero
				<< " ultimo: " << ultimo << endl;
			exit(-1);
		}

		// llamada recursiva

		a->mChild[i] = BuildTree( p_bloque );
		//total += cont;
		total += p_bloque.size();
	}

	if( total != num_dis ) {
		cerr << "ERROR(Vptree): BuildTree(..): total != num_dis" << endl;
		exit(-1);
	}
	
	
	delete [] dist;
	
	return a;
}
    


//-------------------------------------------------------------------
//
int Vptree::TreeDepth( Vptree::Tree *a ) 
{
	if( a == 0 )
		return 0;

	int max_p = 0;
	for( int i = 0; i < a->mNumChild; i++ ) 
	{
		int p = TreeDepth( a->mChild[i] );
		if( p > max_p )
			p = max_p;
	}
	return max_p + 1;
}

