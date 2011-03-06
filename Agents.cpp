
#include <algorithm>
#include <iostream>
#include <map>
#include <cmath>
#include "Agents.h"
#include "Utils.h"
#include "Constants.h"

Action CatAgent::getAction(const GameState& state) {
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

Action MouseAgent::getAction(const GameState& state) {
  const vector<Action> actions = state.getActions(idx_);
  vector<Action> maxActions;
  double maxScore = -kInfinity;
  for (int i = 0; i < actions.size(); i++) {
    const GameState next = state.getNext(actions[i]);
    double score = evaluate(next);
    if (score > maxScore) {
      maxScore = score;
      maxActions.clear();
      maxActions.push_back(actions[i]);
    } else if (score == maxScore) {
      maxActions.push_back(actions[i]);
    }
  }
  random_shuffle(maxActions.begin(), maxActions.end());
  return maxActions[0];
}

double MouseAgent::evaluate(const GameState& state) {
  map<Position, double> distances;
  Utils::mazeDistances(state.getMousePosition(), state, distances);

  double minDistanceToCats = kInfinity;
  double spreadDistanceToCats = 0;
  for (int i = 1; i <= state.getNumAgents() - 1; i++) {
    const Position& catPosition = state.getCatPosition(i);
    const double distance = distances[catPosition];
    if (distance < minDistanceToCats) {
      minDistanceToCats = distance;
    }
    if (distance < kInfinity - 1) {
      spreadDistanceToCats += (1 / distance);
    }
  }
  double score = state.getScore();
  double minDistanceToCheese = kInfinity;

  for (int y = 0; y < kLevelCols; y++) {
    for (int x = 0; x < kLevelCols; x++) {
      if (state.isCheesePosition(x, y)) {
        const double distance = distances[Position(x,y)];
        if (distance < minDistanceToCheese) {
          minDistanceToCheese = distance;
        }
      }
    }
  }
  double cheeseInverse = 1 / (minDistanceToCheese + 1);

  return minDistanceToCats + spreadDistanceToCats + cheeseInverse + score;
}



Action KeyboardAgent::getAction(const GameState& state) {
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
