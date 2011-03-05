
#include <iostream>
#include <map>
#include <set>
#include "Utils.h"
#include "Constants.h"
#include <assert.h>

using namespace std;

double Utils::manhattanDistance(const Position& a,
				const Position& b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}

map<Position, double> Utils::mazeDistances(
  const Position& a,
  const GameState& state) {

  map<Position, double> distances;
  set<Position> nodes;
  for (int y = 0; y < kLevelRows; y++) {
    for (int x = 0; x < kLevelCols; x++) {
      if ((state.get(x, y) == WALL) ||
	  (state.get(x, y) == BLOCK)) {
	continue;
      }
      const Position p(x, y);
      distances[p] = kInfinity;
      nodes.insert(p);
    }
  }
  distances[a] = 0;
  assert(distances.size() == nodes.size());

  while (!nodes.empty()) {
    Position minNode = *(nodes.begin());
    double minDist = kInfinity;
    for (set<Position>::iterator it = nodes.begin();
	 it != nodes.end();
	 it++) {
      const double dist = distances[*it];
      if (dist < minDist) {
	minDist = dist;
	minNode = *it;
      }
    }
    nodes.erase(minNode);
    for (int dy = -1; dy <= 1; dy++) {
      for (int dx = -1; dx <= 1; dx++) {
	if ((dx == 0) && (dy == 0)) {
	  continue;
	}
	Position neighbor(minNode.x + dx, minNode.y + dy);
	if (nodes.count(neighbor)) {
	  double alt = distances[minNode] + 1;
	  if (alt < distances[neighbor]) {
	    assert(distances.count(neighbor));
	    distances[neighbor] = alt;
	  }
	}
      }
    }
  }

  return distances;
}
