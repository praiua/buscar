#include <map>
#include "checkInOracle.h"

map<string, Oracle* (*)( vector<string> )> CheckInOracle::table;
map<string, string> CheckInOracle::tableInfo;
