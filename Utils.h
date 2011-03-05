#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include "GameState.h"

class Utils {
 public:
  static double manhattanDistance(const Position& a,
				  const Position& b);
  static map<Position, double> mazeDistances(
    const Position& a,
    const GameState& state);
};

#endif
