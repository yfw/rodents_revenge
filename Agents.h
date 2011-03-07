
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
  MouseAgent(const vector<double>& weights,
	     const vector<CatAgent>& cats)
    : Agent(0),
      weights_(weights),
      cats_(cats) {}
  Action getAction(const GameState& state) const;
  double evaluate(const GameState& state) const;
 protected:

  double alphaBeta(const GameState& state,
		   int level,
		   double alpha,
		   double beta,
		   Action* actionPtr) const;

  vector<double> weights_;
  vector<CatAgent> cats_;
  static const int depth = 3;
};

class KeyboardAgent : public Agent {
 public:
  KeyboardAgent(const int idx) : Agent(idx) {}
  Action getAction(const GameState& state) const;
};

#endif
