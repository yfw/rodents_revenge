
#ifndef AGENTS_H
#define AGENTS_H

#include <vector>
#include "Constants.h"
#include "GameState.h"

using namespace std;

class Agent {
 public:
  Agent(const int idx) : idx_(idx) {}
  virtual Action getAction(const GameState& state) = 0;

 protected:
  int idx_;
};


class CatAgent : public Agent {
 public:
  CatAgent(const int idx) : Agent(idx) {}
  Action getAction(const GameState& state);
};

class MouseAgent : public Agent {
 public:
  MouseAgent(const int idx) : Agent(idx) {}
  Action getAction(const GameState& state);
  double evaluate(const GameState& state);
};

class KeyboardAgent : public Agent {
 public:
  KeyboardAgent(const int idx) : Agent(idx) {}
  Action getAction(const GameState& state);
};

#endif
