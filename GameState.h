
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <list>
#include <ext/hash_set>
#include "Constants.h"

using namespace std;
using namespace __gnu_cxx;

struct Position {
  Position() {}
  Position(int x_, int y_) : x(x_), y(y_) {}
  bool operator==(const Position& rhs) const {
    return ((x == rhs.x) && (y == rhs.y));
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

struct PositionHash {
  size_t operator()(const Position& pos) const {
    return
      hash<int>()(pos.x) ^ hash<int>()(pos.y);
  }
};

struct Action {
  Action() {}
  Action(const Position& from_, const Position& to_)
  : from(from_),
    to(to_) {}
  bool operator==(const Action& rhs) const {
    return ((from == rhs.from) && (to == rhs.to));
  }
  Position from;
  Position to;
};

class GameState {

 public:

  GameState();
  bool load(const string& fileName);
  void print() const;

  double getDecayedScore() const { return decayedScore_; }
  double getScore() const { return score_; }
  int getTurn() const { return turnIdx_; }
  int getTime() const { return time_; }
  int getNumAgents() const { return agentPositions_.size(); }
  Position getMousePosition() const;
  Position getCatPosition(const int catIdx) const;
  vector<Action> getActions(const int idx) const;
  GameState getNext(const Action& action) const;
  ObjType get(const int x, const int y) const;
  string getGridStr(const Position& position) const;

  bool gameOver() const { return (gameOver_ || time_ > kMaxTime); }
  bool wasCheesed() const { return wasCheesed_; }
  int numCheeseSquashed() const { return numCheeseSquashed_; }
  bool isCheesePosition(const int x, const int y) const;
  bool isCatPosition(const int x, const int y) const;
  bool isCatStuck(const int catIdx) const;

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

  double decayedScore_;
  double score_;
  int turnIdx_;
  int time_;
  bool gameOver_;
  bool wasCheesed_;
  int numCheeseSquashed_;
};


#endif
