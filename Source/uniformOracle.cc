#include <stdlib.h>
#include <math.h>
#include "uniformOracle.h"
#include "checkInOracle.h"

CheckInOracle( 
  UniformOracle, 
  "unif", 
  "Uniform distribution\n\
  -d <int>   Set dimension"
)

UniformOracle::UniformOracle( const char data[]){
  nDist = 0;
  dim = -1;

  int l;
  char token[100];
  while( sscanf(data,"%s%n", token,  &l) == 1 ) {
    data += l;
    if( strcmp( token, "-d" ) == 0 ){
      if( sscanf( data, "%d%n", &dim, &l) != 1) {
        cerr << "ERROR: " << " I couldn't read the '-d' argument" << endl;
        exit(-1);
      }
      data += l;
    } else {
      cerr << "ERROR: " << " unknown option '" << token << "'" << endl;
      exit(-1);
    }
  }

  if( dim < 0 ) {
    cerr << "ERROR: a dimension should be specified" << endl;
    exit(-1);
  }

}    

double UniformOracle::distance( Point p1, Point p2) {
  nDist++;
//  cout << "d( " << p1 << " , " << p2 << " )" << " nDist: " << nDist << endl;
  double res = 0.0;

  for( int i = 0; i < dim; i++ )
    res += pow( data[p1][i] - data[p2][i], 2.0);
  return sqrt(res);
}

Point UniformOracle::newPoint() {
  vector<double> v(dim);
  for( int i = 0; i < dim; i++ ) {
    v[i] = rand()/double(RAND_MAX);
  }
  data.push_back(v);
  return data.size()-1;
}

void UniformOracle::writePoint( Point p ) {
 for( int i = 0; i < dim; i++ )
   cout << data[p][i] << " ";
 cout << endl;
} 

int UniformOracle::distanceComputations() {
  return nDist;
}
