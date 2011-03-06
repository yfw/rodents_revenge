
#ifndef AGENTS_H
#define AGENTS_H

#include <vector>
#include "Constants.h"
#include "GameState.h"

using namespace std;

class Agent {
 public:
  Agent(const int idx) : idx_(idx) {}
  virtual Action getAction(const GameState& state) const = 0;

 protected:
  int idx_;
};


class CatAgent : public Agent {
 public:
  CatAgent(const int idx) : Agent(idx) {}
  Action getAction(const GameState& state) const;
};

class MouseAgent : public Agent {
 public:
  MouseAgent(const vector<double>& weights)
    : Agent(0),
      weights_(weights) {}
  Action getAction(const GameState& state) const;
  double evaluate(const GameState& state) const;
 private:
  vector<double> weights_;
};

class KeyboardAgent : public Agent {
 public:
  KeyboardAgent(const int idx) : Agent(idx) {}
  Action getAction(const GameState& state) const;
};

#endif
