#include <algorithm>
#include <iostream>
#include <map>
#include <cmath>
#include "Agents.h"
#include "Utils.h"
#include "Constants.h"

Action MouseAgent::getAction(const GameState& state) const {
  vector<Action> actions = state.getActions(idx_);
  Action bestAction;
  double value = MouseAgent::alphaBeta(state, 0, -kInfinity, kInfinity, &bestAction);
  cout << "Value: " << value << endl;
  cout << "Action: " << bestAction.to.x << ", " << bestAction.to.y << endl;
  return bestAction;
}

double MouseAgent::alphaBeta(
  const GameState& state,
  int level,
  double alpha,
  double beta,
  Action* bestActionPtr) const {

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

  Action bestAction;
  for (int i = 0; i < actions.size(); i++) {
    GameState successor = state.getNext(actions[i]);
    double nextV = alphaBeta(successor, level + 1, alpha, beta, bestActionPtr);
    if (isMax) {
      if (value < nextV) {
	bestAction = actions[i];
	value = nextV;
      }
      if (value >= beta) {
	break;
      }
      alpha = max(alpha, value);
    } else {
      if (value > nextV) {
	bestAction = actions[i];
	value = nextV;
      }
      if (value <= alpha) {
	break;
      }
      beta = min(beta, value);
    }
  }

  if (level == 0) {
    *bestActionPtr = bestAction;
  }

  return value;
}


double MouseAgent::evaluate(const GameState& state) const {
  map<Position, double> distances;
  Utils::mazeDistances(state.getMousePosition(), state, distances);
  double distanceCatsInverse = 0;
  double freedomCats = 0;
  for (int i = 1; i <= state.getNumAgents() - 1; i++) {
    const Position& catPosition = state.getCatPosition(i);
    const double distance = distances[catPosition];
    if (distance >= 1) {
      distanceCatsInverse += (1 / distance);
    } else {
      return -kInfinity;
    }
    set<Position> visited;
    //const double freedom = Utils::catFreedomScore(catPosition, state, visited);
    freedomCats += Utils::catFreedomScore(catPosition, state, visited);
    cout << "Freedom: " << freedomCats;
    state.print();
    exit(0);
  }

  double distanceCheesesInverse = 0;
  for (int y = 0; y < kLevelCols; y++) {
    for (int x = 0; x < kLevelCols; x++) {
      if (state.isCheesePosition(x, y)) {
        const double distance = distances[Position(x,y)];
	if (distance >= 1) {
	  distanceCheesesInverse += (1 / distance);
	}
      }
    }
  }
  double score = state.getDecayedScore();
  double value =
    //-weights_.at(1) * distanceCatsInverse +
    //weights_.at(2) * distanceCheesesInverse +
    //weights_.at(3) * score + 
    -weights_.at(4) * freedomCats;

  if (true) {
    //cout << "DistanceCatsInverse: " << distanceCatsInverse << endl;
    // << "DistanceCheesesInverse: " << distanceCheesesInverse << endl
    //cout << "Score: " << score << endl;
    //cout << "Value: " << value << endl;
    cerr << "Freedom " << freedomCats << endl;
  }
  usleep(100 *1000);
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
