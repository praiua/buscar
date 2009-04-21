#ifndef vptree_h
#define vptree_h

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <limits>
#include <assert.h>
#include "nnAlg.h"
#include "checkInNNAlg.h"
#include "lib_string.h"


class Vptree: public NNAlg 
{
	
	public:

		Vptree( string data, Oracle *oracle );
		
		~Vptree();
		
		void Insert( Point p );
	
		void InsertBulk( Point p[], int size );
	
		void SearchNN( Point p );

	private:
  
		class Tree {
			public:
				Tree() {};
				~Tree() {
					for( int i = 0; i < mNumChild; i++ ) {
						delete mChild[i];
					}
					delete mRadio_int;
					delete mRadio_ext;
					delete mChild;
				}

				friend int profundidad( Vptree::Tree* a);
				friend void Check( Vptree::Tree *a, int num_punt);
				friend void Check( const Vptree::Tree *a, bool *used, int num_punt);

				int mRep;
				int mNumChild;
				double *mRadio_int;
				double *mRadio_ext;
				Tree **mChild;
		};


		void Search( Point p, const Tree *a, double &disMin, Point &masCecano );

		void Check( const Vptree::Tree *a, bool *used, int num_punt ); 
		void Check( Vptree::Tree *a, int num_punt ); 

		Tree* BuildTree( vector<int> data );
		
		int TreeDepth( Vptree::Tree *a );

		
		
		vector<int> mData;
		
		bool mRule1;
		bool mRule2;
		bool mExtrem;
		
		Tree *mRaiz;
		int  mNumChild;
		int  mLevel;
		int mVisited;

};

#endif
