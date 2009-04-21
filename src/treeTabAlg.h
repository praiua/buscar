#ifndef TreeTab_h
#define TreeTab_h

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <assert.h>
#include "nnAlg.h"
#include "checkInNNAlg.h"
#include "lib_string.h"


class TreeTab: public NNAlg 
{
	public:

 		TreeTab( string data, Oracle * oracle );

		~TreeTab();
		
		void Insert( Point p );
	
		void InsertBulk( Point p[], int size );
	
		void SearchNN( Point p );


	private:  		
  
		class Tree {
			public:
				Tree(): mLeftChild(0), mRightChild(0){};
				~Tree() {
					if( mLeftChild != 0)
						delete mLeftChild;
					if( mRightChild != 0)
						delete mRightChild;
				}

				friend int TreeDepth( TreeTab::Tree *a );

				Tree *mLeftChild;
				Tree *mRightChild;
				int mRep;
		};


		Tree* BuildTree( int p[], const int numP, const int repI);
		Tree* BuildTree();
		void SearchTree( Point p, const Tree *a, double &disMin, int &masCecano);
		void SearchTree( Point p, const Tree *a, const double dis_izq, double &disMin, int &masCecano);
		int TreeDepth( TreeTab::Tree *a );
  
		void BuildTable();
		void SearchTable( Point p, double &disMin, int &masCercano);



		vector<int> mData;
		Tree *mRoot;
		int    mNp;
		double *mTabDis;
		int    *mTabProt;

		int mVisited;

};

#endif
