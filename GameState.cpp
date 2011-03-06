
#include <fstream>
#include <iostream>
#include <assert.h>
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
  score_ = 0;
  turnIdx_ = 0;
  time_ = 0;
  gameOver_ = false;
  wasCheesed_ = false;
}

Position GameState::getMousePosition() const {
  return agentPositions_.at(0);
}

Position GameState::getCatPosition(const int catIdx) const {
  assert((catIdx > 0) && (catIdx < agentPositions_.size()));
  return agentPositions_.at(catIdx);
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

  return actions;
}

vector<Action> GameState::getCatActions(const int catIdx) const {
  assert((catIdx > 0) && (catIdx < agentPositions_.size()));
  vector<Action> actions;
  const Position from = getCatPosition(catIdx);
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
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
  return actions;
}

vector<Action> GameState::getActions(const int idx) const {
  if (idx == 0) {
    return getMouseActions();
  }
  return getCatActions(idx);
}

GameState GameState::getNext(const Action& action) const {
  GameState next = *this;
  vector<Action> actions = getActions(turnIdx_);
  assert(find(actions.begin(), actions.end(), action) != actions.end());
  const Position& to = action.to;
  const Position& from = action.from;
  bool cheesed = true;
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
	next.set(x, y, BLOCK);
      }
    } else if (get(x, y) == CHEESE) {
      next.score_ += 2;
    }
    for (int i = 1; i < agentPositions_.size(); i++) {
      if (getActions(i).size() > 1) {
	cheesed = false;
	break;
      }
    }
    if (cheesed) {
      for (int i = 1; i < agentPositions_.size(); i++) {
	next.set(agentPositions_[i], CHEESE);
	next.score_ += 1;
      }
    }
  } else {
    int x = to.x;
    int y = to.y;
    if (get(x, y) == MOUSE) {
      next.gameOver_ = true;
    }
    cheesed = false;
  }

  next.set(from, NOTHING);
  next.setAgent(to, turnIdx_);
  next.time_++;
  next.turnIdx_ = ((turnIdx_ + 1) % agentPositions_.size());
  next.wasCheesed_ = cheesed;

  if (cheesed) {
    for (int i = 1; i < next.agentPositions_.size(); i++) {
      vector<Position> spawnPoints;
      for (int y = 0; y < kLevelCols; y++) {
	for (int x = 0; x < kLevelRows; x++) {
	  if ((next.get(x, y) == NOTHING) && 
	      (abs(x - to.x) + abs(y - to.y)) > 4) {
	    spawnPoints.push_back(Position(x, y));
	  }
	}
      }
      random_shuffle(spawnPoints.begin(), spawnPoints.end());
      next.setAgent(spawnPoints.at(0), i);
    }
  }

  for (int i = 0; i < next.agentPositions_.size(); i++) {
    ObjType a = next.get(next.agentPositions_[i].x, next.agentPositions_[i].y);
    assert((a == MOUSE) || (a == CAT));
  }

  return next;
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
  for (int i = 0; i < agentPositions_.size(); i++) {
    printAgent(i);
  }
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
