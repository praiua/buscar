#include <map>
#include "checkInNNAlg.h"

map<string, NNAlg* (*)( vector<string>, Oracle* )> CheckInNNAlg::table;
map<string, string> CheckInNNAlg::tableInfo;
