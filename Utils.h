#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <set>
#include "GameState.h"

class Utils {
 public:
  static double manhattanDistance(const Position& a,
				  const Position& b);

  static void shortestDistances(
    const Position& position,
    const GameState& state,
    map<Position, double>* distancesPtr,
    const int cutoff = 1000000);

  static double freedomScore(
    const Position& position,
    const GameState& state,
    const int peakDistance,
    const int cutoff = 83);

  static double mapGetDefault(
    const map<Position, double>& m,
    const Position& key,
    const double defaultValue);
};

#endif
