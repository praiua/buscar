#ifndef checkInOracle_h
#define checkInOracle_h

#include <iostream>
#include <map>
#include <string>
#include "oracle.h"

#define CheckInOracle(CLASS,NAME,INFO) static CheckInOracleDer<CLASS> a(NAME,INFO);

using namespace std;

template <typename T>
Oracle* createOracle(const char data[]) {
  return new T(data);
};

class CheckInOracle {
  public:

  static Oracle* object(string s, const char data[]){
    if( table.count(s) == 1 )
      return table[s]( data );
    return 0;
  }

  static void listInfo() {
    for( map<string,string>::iterator i = tableInfo.begin();
         i != tableInfo.end(); i++ ) {
      cout << i->first << ": " << i->second << endl;
    }
  }

  static map< string, Oracle* (*)(const char []) > table;
  static map< string, string > tableInfo;
};

template< typename T>
class CheckInOracleDer : CheckInOracle {
  public:

  CheckInOracleDer( string name, string info) {
    table[name] = createOracle<T>;
    tableInfo[name] = info;
  }
    
};

#endif
