
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include "GameState.h"
#include "Agents.h"

using namespace std;

int run(const GameState& state,
	const MouseAgent& mouse,
	const vector<CatAgent>& cats) {
  GameState g = state;
  while (!g.gameOver()) {
    system("clear");
    g.print();
    if (g.getTurn() == 0) {
      g = g.getNext(mouse.getAction(g));
    } else {
      g = g.getNext(cats[g.getTurn() - 1].getAction(g));
    }
  }
  system("clear");
  g.print();
  return g.getScore();
}

int main(int argc, char* argv[]) {
  assert(argc == 2);
  GameState g;
  g.load(argv[1]);
  vector<CatAgent> cats;
  for (int i = 1; i < g.getNumAgents(); i++) {
    cats.push_back(CatAgent(i));
  }
  vector<double> weights;
  weights.push_back(1);
  weights.push_back(1);
  weights.push_back(1);
  weights.push_back(1);
  weights.push_back(1);
  weights.push_back(1);
  MouseAgent mouse(weights, cats);
  run(g, mouse, cats);

  return 0;
}
