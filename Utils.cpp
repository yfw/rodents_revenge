
#include <iostream>
#include <ext/hash_set>
#include <ext/hash_map>
#include <set>
#include <queue>
#include <string>

#include "Utils.h"
#include "Constants.h"
#include <assert.h>

using namespace std;

typedef hash_map<string, hash_map<Position, double, PositionHash>, StringHash> cache_map;
const int kCacheSize = 50000;

double Utils::manhattanDistance(const Position& a,
				const Position& b) {
  return max(abs(a.x - b.x), abs(a.y - b.y));
}

void Utils::shortestDistances(
  const Position& position,
  const GameState& state,
  hash_map<Position, double, PositionHash>* distancesPtr,
  const int cutoff) {

  static cache_map cache;
  const string gridStr = state.getGridStr(position);
  cache_map::const_iterator it = cache.find(gridStr);
  if (it != cache.end()) {
    *distancesPtr = hash_map<Position, double, PositionHash>(it->second);
    return;
  }

  queue<pair<Position, double> > Q;
  hash_set<Position, PositionHash> visited;
  Q.push(make_pair(position, 0));
  (*distancesPtr)[position] = 0;
  visited.insert(position);
  while (!Q.empty()) {
    pair<Position, double> front = Q.front();
    const Position& node = front.first;
    const double distance = front.second;
    if (distance > cutoff) {
      break;
    }
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

  if (cache.size() > kCacheSize) {
    cache.clear();
  }
  cache[gridStr] = *distancesPtr;
}

double Utils::freedomScore(
  const Position& position,
  const GameState& state,
  const int peakDistance,
  const int cutoff) {

  static hash_map<string, double, StringHash> cache;
  const string gridStr = state.getGridStr(position);
  hash_map<string, double, StringHash>::const_iterator it = cache.find(gridStr);
  if (it != cache.end()) {
    return it->second;
  }

  queue<pair<Position, double> > Q;
  hash_set<Position, PositionHash> visited;
  vector<double> visitedDistances;
  Q.push(make_pair(position, 0));
  visited.insert(position);
  while (!Q.empty()) {
    pair<Position, double> front = Q.front();
    const Position& node = front.first;
    const double distance = front.second;
    if (distance > cutoff) {
      break;
    }
    Q.pop();
    visitedDistances.push_back(distance);
    for (int dy = -1; dy <= 1; dy++) {
      for (int dx = -1; dx <= 1; dx++) {
	ObjType type = state.get(node.x + dx, node.y + dy);
	if (((dx == 0) && (dy == 0)) ||
	    (type == WALL) ||
	    (type == BLOCK)) { 
	  continue;
	}
	const Position neighbor(node.x + dx, node.y + dy);
	if (!visited.count(neighbor)) {
	  visited.insert(neighbor);
	  Q.push(make_pair(neighbor, distance + 1));
	}
      }
    }
  }

  double score = 0;
  for (int i = 0; i < visitedDistances.size(); i++) {
    double distance = visitedDistances[i];
    score += 1;
    continue;

    if (distance <= peakDistance) {
      score += 1;
    } else if ((distance - peakDistance) <= 10) {
      score += (10 - (distance - peakDistance)) / 20.0;
    }
  }

  if (cache.size() > kCacheSize) {
    cache.clear();
  }
  cache[gridStr] = score;

  return score;
}

double Utils::mapGetDefault(
  const hash_map<Position, double, PositionHash>& m,
  const Position& key,
  const double defaultValue) {

  hash_map<Position, double, PositionHash>::const_iterator it = m.find(key);
  return (it == m.end()) ? defaultValue : it->second;
}

