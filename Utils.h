#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <ext/hash_map>
#include "GameState.h"

using namespace __gnu_cxx;

struct StringHash {
  size_t operator()(const string& x) const {
    return hash<const char*>()(x.c_str());
  }
};

class Utils {
 public:
  static double manhattanDistance(const Position& a,
				  const Position& b);

  static void shortestDistances(
    const Position& position,
    const GameState& state,
    hash_map<Position, double, PositionHash>* distancesPtr,
    const int cutoff = 1000);

  static double freedomScore(
    const Position& position,
    const GameState& state,
    const int peakDistance,
    const int cutoff = 1000);

  static double mapGetDefault(
    const hash_map<Position, double, PositionHash>& m,			      
    const Position& key,
    const double defaultValue);
};

#endif
