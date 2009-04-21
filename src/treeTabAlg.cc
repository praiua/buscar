
#include "treeTabAlg.h"


CheckInNNAlg(
  TreeTab,
  "treetab",
  "Combination of tree and table to search the Nearest Neighbour\n\
  No options"
)



//-------------------------------------------------------------------
//
TreeTab::TreeTab( string data, Oracle * oracle ) 
{
	mOracle = oracle;
	mRoot = 0;
	mTabDis = 0;
	mTabProt = 0;
	
	if( !data.empty() )
	{
                cerr << "ERROR in TreeTab: no options expected (" << data << ")" << endl;
                exit(-1);
	}	
}


//-------------------------------------------------------------------
//
TreeTab::~TreeTab() 
{
	if( mRoot != 0 )
		delete mRoot;
		
	if( mTabDis != 0 )
		delete [] mTabDis;
	
	if( mTabProt != 0 )
		delete [] mTabProt;
}



//-------------------------------------------------------------------
//
void TreeTab::Insert( Point p )
{
	Point aux[] = {p}; 

	InsertBulk( aux, 1 );
}
	



//-------------------------------------------------------------------
//
void TreeTab::InsertBulk( Point p[], int size )
{
    assert( size >= 0 );
	
	for( int i = 0; i < size; i++ ) 
	{
		assert( (unsigned)p[i] == mData.size() );
		mData.push_back( p[i] );
	}
    
    if( mRoot != 0 )
      delete mRoot;
    
    if( mTabDis != 0 )
      delete[] mTabDis;
      
    if( mTabProt != 0 )
      delete[] mTabProt;

    
    mRoot = BuildTree();

    BuildTable();

    mVisited = 0;
}


//-------------------------------------------------------------------
// Point mNNPoint;		// masCercano
// double mNNDistance;	// disMin
void TreeTab::SearchNN( Point p )
{
	SearchTree( p, mRoot, mNNDistance, mNNPoint);
	
	SearchTable( p, mNNDistance, mNNPoint); 
}


//-------------------------------------------------------------------
//
TreeTab::Tree* TreeTab::BuildTree() 
{
	int numP =mData.size();
	int p[ numP ];
	double dis=0.0;

	for( int i = 0; i < numP; i++) {
		p[i] = mData[i];
	}

	int masLejano = p[0];
	double disMasLejano = 0.0;
	
	for( int i = 1; i < numP; i++ ) 
	{
		dis = mOracle->GetDistance( 0, p[i] );
		
		if( dis > disMasLejano ) 
		{
			disMasLejano = dis;
			masLejano = p[i];
		}
	}

	return BuildTree( p, numP, masLejano);
}




//-------------------------------------------------------------------
//
struct st {
	double dis;
	int    prot;
};


static int CompTreeTab( const void *e1, const void *e2) 
{
	const st *p1 = (const st*) e1;
	const st *p2 = (const st*) e2;
	if( p1->dis < p2->dis )
		return -1;
	if( p1->dis > p2->dis )
		return 1;
	return  0;
}



//-------------------------------------------------------------------
//
void TreeTab::BuildTable() 
{
	mNp = mData.size();
	mTabDis = new double[ mNp * mNp ];
	mTabProt = new int[ mNp * mNp ];

	st *buf = new st[ mNp ];

	for( int i = 0; i < mNp; i++ ) 
	{
		for( int j = 0; j < mNp; j++ ) {
			buf[j].dis = mOracle->GetDistance( i, j );
			buf[j].prot = j;
		}

		qsort( buf, mNp, sizeof(st), CompTreeTab );

		for( int j = 0; j < mNp - 1; j++ )
			if( buf[j].dis > buf[j+1].dis ) {
				cerr << "ERROR(TreeTab): BuildTable(...)" << endl;
				exit(-1);
			}

		for( int j = 0; j < mNp; j++ ) {
			mTabDis[i*mNp +j] = buf[j].dis;
			mTabProt[i*mNp +j]  = buf[j].prot;
		}

	}
	
	delete [] buf;
}


//-------------------------------------------------------------------
//
TreeTab::Tree* TreeTab::BuildTree(int p[], const int numP, const int repI)
{
	double disMax = 0.0;
	double dis = 0.0;

	// ver el caso limite
	assert( numP > 0);

	if( numP == 1 ) {
		Tree *a = new Tree;
		if( a == 0 ) {
			cout << "ERROR(TreeTab): BuildTree(..): Not enough memory." << endl;
			exit(-1);
		}
		if( repI != p[0] )
			cout << "(TreeTab): BuildTree(..): different!" << endl;
		a->mRep = p[0];
		a->mLeftChild = 0;
		a->mRightChild = 0;

		return a;
	}

	// calcular el representante derecho (prototipo mas lejano)

	int repD = -1;
	disMax = -1.0;
	for( int i = 0; i < numP; i++ ) 
	{
		if( p[i] != repI ) 
		{
			dis = mOracle->GetDistance( repI, p[i] );
			
			if( dis > disMax ) {
				disMax = dis;
				repD = p[i];
			}
		}
	}

	// calcular distancias

	double *dis_izq = new double[numP];
	double *dis_der = new double[numP];

	for( int i = 0; i < numP; i++ ) {
		dis_izq[i] = mOracle->GetDistance( repI, p[i] );
		dis_der[i] = mOracle->GetDistance( repD, p[i] );
	}


	// repartir los prototipos

	int *pI = new int[numP];
	int *pD = new int[numP];
  
	int numPI = 0;
	int numPD = 0;

	for( int i = 0; i < numP; i++ ) 
	{
		if( p[i] == repI )
			pI[numPI++] = i;
		else if( p[i] == repD )
			pD[numPD++] = i;
		else {
			if( dis_izq[i] < dis_der[i] ) {
				pI[numPI++] = i;
			} else {
				pD[numPD++] = i;
			}
		}
	}

	int *pI2 = new int[numPI];
	for( int i = 0; i < numPI; i++ )
		pI2[i] = p[ pI[i] ];

	int *pD2 = new int[numPD];
	for( int i = 0; i < numPD; i++ )
		pD2[i] = p[ pD[i] ];

	// llamada recursiva

	Tree *hi = BuildTree( pI2, numPI, repI );
	Tree *hd = BuildTree( pD2, numPD, repD );
      

	Tree *a = new Tree;
	if( a == 0 ) {
		cout << "ERROR(TreeTab): BuildTree(..): Not enough memory." << endl;
		exit(-1);
	}
	a->mRep = repI;
	a->mLeftChild = hi;
	a->mRightChild = hd;


	delete [] dis_izq;
	delete [] dis_der;
	delete [] pI;
	delete [] pD;
	delete [] pI2;
	delete [] pD2;
	
	return a;
}
    


//-------------------------------------------------------------------
//
void TreeTab::SearchTree(Point p, const Tree *a, double &dis_min, int &mas_cercano) 
{
	double dis_izq = mOracle->GetDistance( p, a->mLeftChild->mRep );
	
	mas_cercano = a->mLeftChild->mRep;
	dis_min = dis_izq;
	
	SearchTree( p, a, dis_izq, dis_min, mas_cercano);
}



//-------------------------------------------------------------------
//
void TreeTab::SearchTree( Point p, const Tree *a, const double dis_izq, double &dis_min, int &mas_cercano) 
{
	// si estoy en una hoja
	if( a->mLeftChild == 0 && a->mRightChild == 0 ) {   
		return;
	}

	mVisited++;

	double dis_der = mOracle->GetDistance( p, a->mRightChild->mRep );

	if( dis_der < dis_min ){
		dis_min = dis_der;
		mas_cercano = a->mRightChild->mRep;
	}
  
	if( dis_izq < dis_der ) 
	{
		SearchTree( p, a->mLeftChild, dis_izq, dis_min, mas_cercano);
	} 
	else 
	{
		SearchTree( p, a->mRightChild, dis_der, dis_min, mas_cercano);
	}
}



//-------------------------------------------------------------------
//
int TreeTab::TreeDepth( TreeTab::Tree *a) 
{
	if( a == 0 )
		return 0;

	int p1 = TreeDepth( a->mLeftChild );
	int p2 = TreeDepth( a->mRightChild );
	if( p1 > p2 )
		return p1 + 1;
	else
		return p2 + 1;
}



//-------------------------------------------------------------------
//
void TreeTab::SearchTable( Point p, double &disMin, int &masCercano) 
{
	int i = masCercano;
	st *buf = new st[ mNp ];

	for( int j = 0; j < mNp; j++ ) {
		buf[j].dis = fabs( mTabDis[i*mNp+j] - disMin );
		buf[j].prot = mTabProt[i*mNp+j];
	}

	qsort( buf, mNp, sizeof(st), CompTreeTab );

	for( int j = 0; j < mNp; j++ ) 
	{
		int p2 = buf[j].prot;
		
		double d = mOracle->GetDistance( p, p2 );
		
		if( d < disMin ) {
			disMin = d;
			masCercano = p2;
		}
		
		if( disMin < buf[j].dis )
			break;
	}
	
	delete [] buf;
}



