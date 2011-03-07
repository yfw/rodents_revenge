#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include "GameState.h"
#include "Agents.h"
#include <time.h>
#include "GeneticUtils.h"
#include "Constants.h"

using namespace std;

int run(const GameState& state,
	const MouseAgent& mouse,
	const vector<CatAgent>& cats) {
  GameState g = state;
  while (!g.gameOver()) {
    //system("clear");
    g.print();
    cout << "Value: " << mouse.evaluate(g) << endl;
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
  assert(argc >= 2);
  string map = argv[1];
  if (argc == 3) {
    string flag = argv[2];
    if (flag == "--weights") {
      vector<double> weights;
      GeneticUtils::getWeights(weights, map);
    }
  } else {
    GameState g;
    g.load(map);
    vector<CatAgent> cats;
    for (int i = 1; i < g.getNumAgents(); i++) {
      cats.push_back(CatAgent(i));
    }
    vector<double> weights(kInitialWeights, kInitialWeights + sizeof(kInitialWeights) / sizeof(double));
    MouseAgent mouse(weights, cats);

    time_t seconds = time(NULL);
    run(g, mouse, cats);
    cout << time(NULL) - seconds;
    return 0;
  }

  return 0;
}
