#include <algorithm>
#include <iostream>
#include <map>
#include <cmath>
#include "Agents.h"
#include "Utils.h"
#include "Constants.h"

Action MouseAgent::getAction(const GameState& state) const {
  vector<Action> actions = state.getActions(idx_);
  double value;
  return MouseAgent::alphaBeta(state, 0, &value, -kInfinity, kInfinity);;
}

Action MouseAgent::alphaBeta(
  const GameState& state,
  int level,
  double* value,
  double alpha,
  double beta) const {

  if (state.wasCheesed() ||
      state.gameOver() ||
      level == (depth * state.getNumAgents())) {
    *value = evaluate(state);
    Action a = Action();
    return a;
  }

  int agentIndex = level % state.getNumAgents();
  bool takeMax = (agentIndex == 0);
  if (agentIndex > 0) { // Cat agent
    *value = kInfinity;
  } else { // Mouse agent
    *value = -kInfinity;
  }

  vector<Action> actions;
  if (agentIndex == 0) {
    actions = state.getActions(agentIndex);      
  } else {
    actions.push_back(cats_[agentIndex - 1].getAction(state));
  }

  Action bestAction;
  for (int i = 0; i < actions.size(); i++) {
    GameState successor = state.getNext(actions[i]);
    double nextV;
    MouseAgent::alphaBeta(successor, level + 1, &nextV, alpha, beta);
    if ((takeMax && nextV > *value) ||
        (!takeMax && nextV < *value)) {
      *value = nextV;
      bestAction = actions[i];
    }
    // Prune if possible
    if ((takeMax && *value >= beta) ||
	(!takeMax && *value <= alpha)) {
      break;
    }
    // Update alpha & beta
    if (!takeMax) {
      beta = min(beta, *value);
    } else {
      alpha = max(alpha, *value);
    }
  }

  return bestAction;
}


double MouseAgent::evaluate(const GameState& state) const {
  map<Position, double> distances;
  Utils::mazeDistances(state.getMousePosition(), state, distances);
  double distanceCatMin = kInfinity;
  double distanceCatsInverse = 0;
  for (int i = 1; i <= state.getNumAgents() - 1; i++) {
    const Position& catPosition = state.getCatPosition(i);
    const double distance = distances[catPosition];
    if (distance < distanceCatMin) {
       distanceCatMin = distance;
    }
    if (distance < kInfinity - 1) {
      distanceCatsInverse += (1 / distance);
    }
  }
  
  double distanceCheeseMin = kInfinity;
  double distanceCheesesInverse = 0;
  for (int y = 0; y < kLevelCols; y++) {
    for (int x = 0; x < kLevelCols; x++) {
      if (state.isCheesePosition(x, y)) {
        const double distance = distances[Position(x,y)];
        if (distance < distanceCheeseMin) {
          distanceCheeseMin = distance;
        }
	if (distance < kInfinity - 1) {
	  distanceCheesesInverse += (1 / distance);
	}
      }
    }
  }
  double score = state.getScore();

  double value =
    weights_.at(0) * distanceCatMin +
    weights_.at(1) * distanceCatsInverse +
    weights_.at(2) * distanceCheeseMin +
    weights_.at(3) * distanceCheesesInverse * 
    weights_.at(4) * score;

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
