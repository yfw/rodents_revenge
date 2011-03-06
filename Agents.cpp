#include <algorithm>
#include <iostream>
#include <ext/hash_map>
#include <cmath>
#include "Agents.h"
#include "Utils.h"
#include "Constants.h"

Action MouseAgent::getAction(const GameState& state) const {
  //cout << "Value: " << MouseAgent::evaluate(state) << endl;
  //exit(0);
  vector<Action> actions = state.getActions(idx_);
  Action bestAction;
  double value = MouseAgent::alphaBeta(state, 0, -kInfinity, kInfinity, &bestAction);
  cout << "Value: " << evaluate(state);
  //cout << "Action: " << bestAction.to.x << ", " << bestAction.to.y << endl;
  //exit(0);
  return bestAction;
}

double MouseAgent::alphaBeta(
  const GameState& state,
  int level,
  double alpha,
  double beta,
  Action* actionPtr) const {

  if (state.wasCheesed() ||
      state.gameOver() ||
      level == (depth * state.getNumAgents())) {
    return evaluate(state);
  }

  int agentIndex = level % state.getNumAgents();
  bool isMax = (agentIndex == 0);
  double value = isMax ? -kInfinity : kInfinity;
  vector<Action> actions = isMax
    ? state.getActions(agentIndex)
    : vector<Action>(1, cats_[agentIndex - 1].getAction(state));

  vector<Action> bestActions;
  for (int i = 0; i < actions.size(); i++) {
    const GameState successor = state.getNext(actions[i]);
    double nextV = alphaBeta(successor, level + 1, alpha, beta, actionPtr);
    if (isMax) {
      if (value < nextV) {
	if (level == 0) {
	  bestActions.clear();
	  bestActions.push_back(actions[i]);
	}
	value = nextV;
      } else if ((value == nextV) && (level == 0)) {
	bestActions.push_back(actions[i]);
      }
      if (value >= beta) {
	break;
      }
      alpha = max(alpha, value);
    } else {
      if (value > nextV) {
	if (level == 0) {
	  bestActions.clear();
	  bestActions.push_back(actions[i]);
	}
	value = nextV;
      } else if ((value == nextV) && (level == 0)) {
	bestActions.push_back(actions[i]);
      }
      if (value <= alpha) {
	break;
      }
      beta = min(beta, value);
    }
  }
  if (level == 0) {
    *actionPtr = bestActions.at(rand() % bestActions.size());
  }
  return value;
}


double MouseAgent::evaluate(const GameState& state) const {
  hash_map<Position, double, PositionHash> distances;
  Utils::shortestDistances(state.getMousePosition(), state, &distances);

  if (state.gameOver()) {
    return -100000;
  }

  if (state.wasCheesed()) {
    return 100000;
  }

  double distanceCatsInverse = 0;
  double freedomScoreCats = 0;
  double manhattanDistanceNearestCat = 100;
  for (int i = 1; i <= state.getNumAgents() - 1; i++) {
    const Position& catPosition = state.getCatPosition(i);
    const double distance = Utils::mapGetDefault(distances,
						 catPosition,
						 kInfinity);
    const double manhattanDistance =
      Utils::manhattanDistance(state.getMousePosition(),
			       catPosition);

    if (distance >= 1) {
      distanceCatsInverse += (1 / distance);
    }

    if (!state.isCatStuck(i) &&
	(manhattanDistance < manhattanDistanceNearestCat)) {
      manhattanDistanceNearestCat = manhattanDistance;
    }

    freedomScoreCats += Utils::freedomScore(catPosition, state, 6);
  }

  double distanceCheesesInverse = 0;
  for (int y = 0; y < kLevelCols; y++) {
    for (int x = 0; x < kLevelCols; x++) {
      if (state.isCheesePosition(x, y)) {
	const double distance = Utils::mapGetDefault(distances,
						     Position(x, y),
						     kInfinity);
	if (distance >= 1) {
	  distanceCheesesInverse += (1 / distance);
	}
      }
    }
  }

  double score = state.getDecayedScore();
  double value =
    -weights_.at(0) * distanceCatsInverse * 2 +
    //-weights_.at(1) * manhattanDistanceNearestCat * 0.001 +
    -weights_.at(2) * freedomScoreCats +
    weights_.at(3) * distanceCheesesInverse * 5 +
    weights_.at(4) * score * 50;

  if (true) {
    //cerr << "Score: " << score << endl;
    //cerr << "Value: " << manhattanDistanceNearestCat << endl;
  }

  return value;
}

Action KeyboardAgent::getAction(const GameState& state) const {
  string input;
  bool valid = false;

  while (true) {
    cin >> input;
    char dir = input[0];
    int dx = 0;
    int dy = 0;
    if (dir == 'w') {
      dy = -1;
    } else if (dir == 's') {
      dy = 1;
    } else if (dir == 'a') {
      dx = -1;
    } else if (dir == 'd') {
      dx = 1;
    } else {
      continue;
    }

    vector<Action> actions = state.getActions(idx_);
    unsigned int i;
    for (i = 0; i < actions.size(); i++) {
      int adx = actions[i].to.x - actions[i].from.x;
      int ady = actions[i].to.y - actions[i].from.y;
      if ((adx == dx) && (ady == dy)) {
	valid = true;
	break;
      }
    }

    if (valid) {
      return actions[i];
    }
  }
}

Action CatAgent::getAction(const GameState& state) const {
  const vector<Action> actions = state.getActions(idx_);
  const Position& mousePosition = state.getMousePosition();
  Action minDistAction;
  double minDist = kInfinity;
  for (int i = 0; i < actions.size(); i++) {
    double dist = Utils::manhattanDistance(actions[i].to, mousePosition);
    if (dist < minDist) {
      minDist = dist;
      minDistAction = actions[i];
    }
  }
  return minDistAction;
}
