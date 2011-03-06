#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <set>
#include "GameState.h"

class Utils {
 public:
  static double manhattanDistance(const Position& a,
				  const Position& b);
  static void mazeDistances(
    const Position& a,
    const GameState& state,
    map<Position, double>& distances);

  static int catFreedomScore(const Position& position,
			     const GameState& state,
			     set<Position>& visited);
};

#endif
