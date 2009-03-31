#include <map>
#include "checkInNNAlg.h"

map<string, NNAlg* (*)(Oracle*, const char d[])> CheckInNNAlg::table;
map<string, string> CheckInNNAlg::tableInfo;
