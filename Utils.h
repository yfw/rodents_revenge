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
    map<Position, double>* distancesPtr);

  static double freedomScore(
    const Position& position,
    const GameState& state,
    const int cutoff);

  static double mapGetDefault(
    const map<Position, double>& m,
    const Position& key,
    const double defaultValue);
};

#endif
