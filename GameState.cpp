
#include <fstream>
#include <iostream>
#include <math.h>
#include <assert.h>
#include <sstream>
#include "GameState.h"

GameState::GameState() {}

bool GameState::load(const string& fileName) {
  grid_ = vector<ObjType>(kLevelRows * kLevelCols, NOTHING);
  ifstream file(fileName.c_str());
  assert(file.is_open());

  vector<string> level;
  while (file.good()) {
    string line;
    getline(file, line);
    if (line.size() == kLevelCols) {
      level.push_back(line);
    }
  }
  assert(level.size() == kLevelRows);

  for (int y = 0; y < kLevelCols; y++) {
    for (int x = 0; x < kLevelCols; x++) {
      set(x, y, level[y][x]);
      if (level[y][x] == MOUSE) {
        agentPositions_.insert(agentPositions_.begin(),
			       Position(x, y));
      } else if (level[y][x] == CAT) {
	agentPositions_.push_back(Position(x, y));
      }
    }
  }
  while (agentPositions_.size() < kCats + 1) {
    agentPositions_.push_back(Position(0, 0));
    spawnCat(agentPositions_.size() - 1);
  }

  decayedScore_ = 0;
  score_ = 0;
  turnIdx_ = 0;
  time_ = 0;
  gameOver_ = false;
  wasCheesed_ = false;
  numCheeseSquashed_ = 0;
  numBlocksMoved_ = 0;
}

Position GameState::getMousePosition() const {
  return agentPositions_.at(0);
}

Position GameState::getCatPosition(const int catIdx) const {
  assert((catIdx > 0) && (catIdx < agentPositions_.size()));
  return agentPositions_.at(catIdx);
}

bool GameState::isCheesePosition(const int x, const int y) const {
  return get(x, y) == CHEESE;
}

bool GameState::isCatPosition(const int x, const int y) const {
  return get(x, y) == CAT;
}

bool GameState::isCatStuck(const int catIdx) const {
  vector<Action> actions = getActions(catIdx);
  Position position = getPosition(catIdx);
  return ((actions.size() == 1) && (actions[0] == Action(position, position)));
}

Position GameState::getPosition(const int idx) const {
  if (idx == 0) {
    return getMousePosition();
  }
  return getCatPosition(idx);
}

vector<Action> GameState::getMouseActions() const {
  vector<Action> actions;
  const Position from = getMousePosition();
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if ((dx == 0) && (dy == 0)) {
	//continue;
      }
      int x = from.x + dx;
      int y = from.y + dy;
      Position to = Position(x, y);
      while (get(x, y) == BLOCK) {
        x += dx;
        y += dy;
      }
      if ((get(x, y) == WALL) ||
          (get(x, y) == CAT)) {
        continue;
      }
      actions.push_back(Action(from, to));
    }
  }
  if (actions.size() == 0) {
    actions.push_back(Action(from, from));
  }
  return actions;
}

vector<Action> GameState::getCatActions(const int catIdx) const {
  assert((catIdx > 0) && (catIdx < agentPositions_.size()));
  vector<Action> actions;
  const Position from = getCatPosition(catIdx);
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if ((dx == 0) && (dy == 0)) {
	continue;
      }
      int x = from.x + dx;
      int y = from.y + dy;
      Position to = Position(x, y);
      if ((get(x, y) == NOTHING) ||
          (get(x, y) == MOUSE) ||
          (to == from)) {
        actions.push_back(Action(from, to));
      }
    }
  }
  if (actions.size() == 0) {
    actions.push_back(Action(from, from));
  }
  return actions;
}

vector<Action> GameState::getActions(const int idx) const {
  if (idx == 0) {
    return getMouseActions();
  }
  return getCatActions(idx);
}

string GameState::getGridStr(const Position& position) const {
  stringstream out;
  string gridStr;
  gridStr.insert(gridStr.begin(), grid_.begin(), grid_.end());
  out << gridStr << position.x << ":" << position.y;
  return out.str();
}

GameState GameState::getNext(const Action& action) const {
  GameState next = *this;
  vector<Action> actions = getActions(turnIdx_);
  assert(find(actions.begin(), actions.end(), action) != actions.end());
  const Position& to = action.to;
  const Position& from = action.from;
  bool cheesed = false;
  int numSquashed = 0;
  next.time_++;
  next.turnIdx_ = ((turnIdx_ + 1) % agentPositions_.size());

  if (turnIdx_ == 0) {
    int x = to.x;
    int y = to.y;
    if (get(x, y) == BLOCK) {
      int dx = to.x - from.x;
      int dy = to.y - from.y;
      while (get(x, y) == BLOCK) {
        x += dx;
        y += dy;
      }
      if ((get(x, y) == NOTHING) ||
          (get(x, y) == CHEESE)) {
	if (get(x, y) == CHEESE) {
	  numSquashed += 1;
	}
	next.numBlocksMoved_ += pow(0.99999, next.time_);
        next.set(x, y, BLOCK);
      }
    } else if (get(x, y) == CHEESE) {
      next.score_ += kScoreEatCheese;
      next.decayedScore_ += (kScoreEatCheese * pow(0.99999, next.time_));
    }
    for (int i = 1; i < agentPositions_.size(); i++) {
      if (isCatStuck(i)) {
        next.set(agentPositions_[i], CHEESE);
        next.score_ += kScoreTrapCat;
	next.decayedScore_ += (kScoreTrapCat * pow(0.99990, next.time_));
	cheesed = true;
      }
    }
  } else {
    int x = to.x;
    int y = to.y;
    if (get(x, y) == MOUSE) {
      next.gameOver_ = true;
    }
  }

  next.set(from, NOTHING);
  next.setAgent(to, turnIdx_);
  next.wasCheesed_ = cheesed;
  next.numCheeseSquashed_ = numCheeseSquashed_ + numSquashed;

  if (cheesed) {
    for (int i = 1; i < next.agentPositions_.size(); i++) {
      if (isCatStuck(i)) {
	next.spawnCat(i);
      }
    }
  }

  for (int i = 0; i < next.agentPositions_.size(); i++) {
    ObjType a = next.get(next.agentPositions_[i].x, next.agentPositions_[i].y);
    assert((a == MOUSE) || (a == CAT));
  }

  return next;
}

void GameState::spawnCat(const int catIdx) {
  vector<Position> spawnPoints;
  Position to = getMousePosition();
  for (int y = 0; y < kLevelCols; y++) {
    for (int x = 0; x < kLevelRows; x++) {
      if ((get(x, y) == NOTHING) &&
	  (abs(x - to.x) + abs(y - to.y)) >= kSpawnDistanceMin) {
	spawnPoints.push_back(Position(x, y));
      }
    }
  }
  setAgent(spawnPoints.at(rand() % spawnPoints.size()), catIdx);
}

void GameState::print() const {
  for (int y = 0; y < kLevelCols; y++) {
    for (int x = 0; x < kLevelCols; x++) {
      cout << get(x, y);
    }
    cout << endl;
  }

  cout << "Score: " << score_ << endl;
  cout << "Time: " << time_ << endl;
  //cout << "Blocks Moved: " << numBlocksMoved_ << endl;
  //for (int i = 0; i < agentPositions_.size(); i++) {
  //printAgent(i);
  //}
}

void GameState::printAgent(const int idx) const {
  Position position = getPosition(idx);
  if (idx == 0) {
    cout << "Mouse" << endl;
  } else {
    cout << "Cat" << idx << endl;
  }
  cout << "  Position: (" << position.x << ", " << position.y << ")" << endl;

  vector<Action> actions = getActions(idx);
  cout << "  Actions: ";
  for (int i = 0; i < actions.size(); i++) {
    cout << "(" << actions[i].to.x << ", " << actions[i].to.y << ") ";
  }
  cout << endl;  
}

void GameState::set(const int x, const int y, const ObjType t) {
  assert((x >= 0) && (x < kLevelRows) &&
      (y >= 0) && (y < kLevelCols));
  grid_.at(y * kLevelCols + x) = t;
}

void GameState::set(const Position& position, const ObjType t) {
  set(position.x, position.y, t);
}

void GameState::setAgent(const Position& position, const int idx) {
  set(position, (idx == 0) ? MOUSE : CAT);
  agentPositions_.at(idx) = position;
}

ObjType GameState::get(const int x, const int y) const {
  assert((x >= 0) && (x < kLevelRows) &&
      (y >= 0) && (y < kLevelCols));
  return grid_.at(y * kLevelCols + x);
}
