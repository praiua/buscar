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
NNAlg* CreateNNAlg( vector<string> data, Oracle *o ) 
{
	return new T( data, o);
};



class CheckInNNAlg 
{
	public:

		static NNAlg* Object( vector<string> data, Oracle* o )
		{
			if( table.count( data[0] ) == 1 )
				return table[ data[0] ]( data, o );
			return 0;
		}

		static void ListInfo(string name="") 
		{
			if(name != "")
				cout <<  "* " << name << ": " << tableInfo[name] << endl;
			else {
				for( map<string,string>::iterator i = tableInfo.begin();
					i != tableInfo.end(); i++ ) 
				{
						cout << "* " << i->first << ": " << i->second << endl << endl;
				}
			}
		}

		static map< string, NNAlg* (*)( vector<string>, Oracle* ) > table;
		static map< string, string > tableInfo;
};



template< typename T>
class CheckInNNAlgDer : CheckInNNAlg 
{
	public:

		CheckInNNAlgDer( string name, string info) 
		{
			table[name] = CreateNNAlg<T>;
			tableInfo[name] = info;
		}
    
};

#endif
