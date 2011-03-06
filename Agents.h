
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
<<<<<<< HEAD
  MouseAgent(const vector<double>& weights,
	     const vector<CatAgent>& cats)
=======
  MouseAgent(const vector<double>& weights)
>>>>>>> e6c1c34826f54dca246d0d5606d07c05b583cbce
    : Agent(0),
      weights_(weights),
      cats_(cats) {}
  Action getAction(const GameState& state) const;
 protected:
<<<<<<< HEAD
  double evaluate(const GameState& state) const;
  Action alphaBeta(const GameState& state,
		   int level,
		   double* value,
		   double alpha,
		   double beta) const;

  vector<double> weights_;
  vector<CatAgent> cats_;
  static const int depth = 3;
=======
  vector<double> weights_;
  static const int depth = 1;
  double evaluate(const GameState& state) const;
  Action minMax(const GameState& state, int level, double* value) const;
>>>>>>> e6c1c34826f54dca246d0d5606d07c05b583cbce
};

class KeyboardAgent : public Agent {
 public:
  KeyboardAgent(const int idx) : Agent(idx) {}
  Action getAction(const GameState& state) const;
};

#endif
