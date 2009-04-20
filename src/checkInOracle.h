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
Oracle* CreateOracle( vector<string> data ) 
{
	return new T(data);
};



class CheckInOracle 
{
	public:

		typedef map< string, Oracle* (*)( vector<string> ) > table_t;
		typedef map< string, string > tableInfo_t;

//
//      This is done in this ugly way to avoid the "static initialization order fiasco"
//

                static table_t& table(){
                        static table_t *a = new table_t;
                        return *a;
                }

                static tableInfo_t& tableInfo(){
                        static tableInfo_t *a = new tableInfo_t;
                        return *a;
                }
		static Oracle* Object( vector<string> data )
		{
			if( table().count( data[0] ) == 1 )
				return table()[ data[0] ]( data );
			return 0;
		}

		static void ListInfo(string name="") 
		{
			if(name != "")
				cout << "* " << name << ": " << tableInfo()[name] << endl;
			else {
				for( map<string,string>::iterator i = tableInfo().begin();
					i != tableInfo().end(); i++ ) 
				{			
						cout << "* " << i->first << ": " << i->second << endl << endl;
				}
			}
		}


};



template< typename T>
class CheckInOracleDer : CheckInOracle 
{
	public:

		CheckInOracleDer( string name, string info) 
		{
			table()[name] = CreateOracle<T>;
			tableInfo()[name] = info;
		}    
};

#endif
