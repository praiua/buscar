#ifndef checkInNNAlg_h
#define checkInNNAlg_h

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "nnAlg.h"

using namespace std;

#define CheckInNNAlg(CLASS,NAME,INFO) \
        static CheckInNNAlgDer<CLASS> a(NAME,INFO);

template <typename T>
NNAlg* CreateNNAlg( string data, Oracle *o ) 
{
	return new T( data, o);
};



class CheckInNNAlg 
{
	public:
		typedef map< string, NNAlg* (*)( string, Oracle* ) > table_t;
		typedef map< string, string > tableInfo_t;
//
// 	This is done in this ugly way to avoid the "static initialization order fiasco"
//
		static table_t& table(){
			static table_t *a = new table_t;
			return *a;
		}

		static tableInfo_t& tableInfo(){
			static tableInfo_t *a = new tableInfo_t;
			return *a;
		}

		static NNAlg* Object( string algName, string data, Oracle* o )
		{
			if( table().count( algName ) == 1 )
				return table()[ algName ]( data, o );
			return 0;
		}

		static void ListInfo(string name = "") 
		{
			if(name != "")
				cout <<  "* " << name << ": " << tableInfo()[name] << endl;
			else {
				for(tableInfo_t::iterator i = tableInfo().begin();
					i != tableInfo().end(); i++ ) 
				{
					cout << "* " << i->first << ": " << i->second << endl << endl;
				}
			}
		}


};



template< typename T>
class CheckInNNAlgDer : CheckInNNAlg 
{
	public:

		CheckInNNAlgDer( string name, string info) 
		{
			table()[name] = CreateNNAlg<T>;
			tableInfo()[name] = info;
		}
    
};

#endif
