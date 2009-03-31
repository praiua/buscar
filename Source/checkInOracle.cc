#include <map>
#include "checkInOracle.h"

map<string, Oracle* (*)(const char d[])> CheckInOracle::table;
map<string, string> CheckInOracle::tableInfo;
