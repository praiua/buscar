#ifndef checkInNNAlg_h
#define checkInNNAlg_h

#include <iostream>
#include <map>
#include <string>
#include "nnAlg.h"

#define CheckInNNAlg(CLASS,NAME,INFO) \
        static CheckInNNAlgDer<CLASS> a(NAME,INFO);

using namespace std;

template <typename T>
NNAlg* createNNAlg( Oracle *o, const char data[]) {
  return new T( o, data);
};

class CheckInNNAlg {
  public:

  static NNAlg* object(string s, Oracle* o, const char data[]){
    if( table.count(s) == 1 )
      return table[s]( o, data );
    return 0;
  }

  static void listInfo() {
    for( map<string,string>::iterator i = tableInfo.begin();
         i != tableInfo.end(); i++ ) {
      cout << i->first << ": " << i->second << endl;
      cout << endl;
    }
  }

  static map< string, NNAlg* (*)(Oracle*, const char []) > table;
  static map< string, string > tableInfo;
};

template< typename T>
class CheckInNNAlgDer : CheckInNNAlg {
  public:

  CheckInNNAlgDer( string name, string info) {
    table[name] = createNNAlg<T>;
    tableInfo[name] = info;
  }
    
};

#endif
