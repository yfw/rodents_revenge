
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include "Constants.h"

using namespace std;

struct Position {
  Position() {}
  Position(int x_, int y_) : x(x_), y(y_) {}
  bool operator==(const Position& other) const {
    return ((x == other.x) && (y == other.y));
  }

  bool operator<(const Position& rhs) const {
    if (y != rhs.y) {
      return y < rhs.y;
    }
    return x < rhs.x;
  }   

  int x;
  int y;
};

struct Action {
  Action() {}
  Action(const Position& from_, const Position& to_)
  : from(from_),
    to(to_) {}
  bool operator==(const Action& other) const {
    return ((from == other.from) && (to == other.to));
  }
  Position from;
  Position to;
};

class GameState {

 public:

  GameState();
  bool load(const string& fileName);
  void print() const;

  int getScore() const { return score_; }
  int getTurn() const { return turnIdx_; }
  int getNumAgents() const { return agentPositions_.size(); }
  bool gameOver() const { return (gameOver_ || time_ > kMaxTime); }
  bool wasCheesed() const { return wasCheesed_; }

  Position getMousePosition() const;
  Position getCatPosition(const int catIdx) const;
  vector<Action> getActions(const int idx) const;

  GameState getNext(const Action& action) const;
  ObjType get(const int x, const int y) const;

 private:
  Position getPosition(const int idx) const;
  vector<Action> getMouseActions() const;
  vector<Action> getCatActions(const int catIdx) const;

  void set(const int x, const int y, const ObjType t);
  void set(const Position& pos, const ObjType t);
  void setAgent(const Position& pos, const int idx);

  void printAgent(const int idx) const;

  vector<ObjType> grid_;
  vector<Position> agentPositions_;
  int score_;
  int turnIdx_;
  int time_;
  bool gameOver_;
  bool wasCheesed_;
};


#endif
