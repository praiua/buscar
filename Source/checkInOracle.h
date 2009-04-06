#ifndef checkInOracle_h
#define checkInOracle_h

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "oracle.h"

using namespace std;



#define CheckInOracle(CLASS,NAME,INFO) \
		static CheckInOracleDer<CLASS> a(NAME,INFO);



template <typename T>
Oracle* createOracle( vector<string> data ) 
{
  return new T(data);
};



class CheckInOracle 
{
	public:

		static Oracle* Object( vector<string> data )
		{
			if( table.count( data[0] ) == 1 )
				return table[ data[0] ]( data );
			return 0;
		}

		static void ListInfo() 
		{
			for( map<string,string>::iterator i = tableInfo.begin();
				i != tableInfo.end(); i++ ) 
			{			
					cout << "* " << i->first << ": " << i->second << endl << endl;
			}
		}

		static map< string, Oracle* (*)( vector<string> ) > table;
		static map< string, string > tableInfo;
};



template< typename T>
class CheckInOracleDer : CheckInOracle 
{
  public:

		CheckInOracleDer( string name, string info) 
		{
			table[name] = createOracle<T>;
			tableInfo[name] = info;
		}    
};

#endif
