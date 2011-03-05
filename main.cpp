
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include "GameState.h"
#include "Agents.h"

using namespace std;

int main(int argc, char* argv[]) {

  assert(argc == 2);
  GameState g;
  g.load(argv[1]);

  string input;
  MouseAgent mouse(0);
  vector<CatAgent> cats;
  for (int i = 1; i < g.getNumAgents(); i++) {
    cats.push_back(CatAgent(i));
  }

  while (!g.gameOver()) {
    system("clear");
    g.print();
    if (g.getTurn() == 0) {
      g = g.getNext(mouse.getAction(g));
    } else {
      g = g.getNext(cats[g.getTurn() - 1].getAction(g));
    }
    // usleep(1 * 1000);
  }
  g.print();

  return 0;
}
