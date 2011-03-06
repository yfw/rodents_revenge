
#include <iostream>
#include <map>
#include <set>
#include <queue>

#include "Utils.h"
#include "Constants.h"
#include <assert.h>

using namespace std;

double Utils::manhattanDistance(const Position& a,
				const Position& b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}

void Utils::shortestDistances(
  const Position& position,
  const GameState& state,
  map<Position, double>* distancesPtr) {

  queue<pair<Position, double> > Q;
  set<Position> visited;
  Q.push(make_pair(position, 0));
  visited.insert(position);
  while (!Q.empty()) {
    pair<Position, double> front = Q.front();
    const Position& node = front.first;
    const double distance = front.second;
    (*distancesPtr)[node] = distance;
    Q.pop();
    for (int dy = -1; dy <= 1; dy++) {
      for (int dx = -1; dx <= 1; dx++) {
	ObjType type = state.get(node.x + dx, node.y + dy);
	if (((dx == 0) && (dy == 0)) ||
	    (type == WALL) ||
	    (type == BLOCK)) { 
	  continue;
	}
	Position neighbor(node.x + dx, node.y + dy);
	if (!visited.count(neighbor)) {
	  visited.insert(neighbor);
	  Q.push(make_pair(neighbor, distance + 1));
	}
      }
    }
  }
}

static double freedomScore(
  const Position& position,
  const GameState& state,
  const int cutoff) {

  queue<pair<Position, double> > Q;
  set<Position> visited;
  Q.push(make_pair(position, 0));
  visited.insert(position);
  while (!Q.empty()) {
    pair<Position, double> front = Q.front();
    const Position& node = front.first;
    const double distance = front.second;
    Q.pop();
    for (int dy = -1; dy <= 1; dy++) {
      for (int dx = -1; dx <= 1; dx++) {
	ObjType type = state.get(node.x + dx, node.y + dy);
	if (((dx == 0) && (dy == 0)) ||
	    (type == WALL) ||
	    (type == BLOCK)) { 
	  continue;
	}
	Position neighbor(node.x + dx, node.y + dy);
	if (!visited.count(neighbor)) {
	  visited.insert(neighbor);
	  Q.push(make_pair(neighbor, distance + 1));
	}
      }
    }
  }

  return Q.size();
}

double Utils::mapGetDefault(
  const map<Position, double>& m,
  const Position& key,
  const double defaultValue) {

  map<Position, double>::const_iterator it = m.find(key);
  return (it == m.end()) ? defaultValue : it->second;
}
