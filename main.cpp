#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "Agents.h"
#include "GeneticUtils.h"
#include "Constants.h"
#include "Renderer.h"

using namespace std;

int run(GameState state,
	const MouseAgent& mouse,
	const vector<CatAgent>& cats) {

  sf::RenderWindow app(
    sf::VideoMode(kLevelRows * 16, kLevelCols * 16, 32),
    "Rodent's Revenge");

  sf::Clock clock;

  Renderer renderer;

  while (!state.gameOver() && app.IsOpened()) {
    sf::Event event;
    while (app.GetEvent(event)) {
      if (event.Type == sf::Event::Closed) {
	app.Close();
      }
    }

    renderer.render(app, state);
    state.print();
    if (clock.GetElapsedTime() > 0.0001) {
      clock.Reset();
      if (state.getTurn() == 0) {
	state = state.getNext(mouse.getAction(state));
      } else {
	state = state.getNext(cats[state.getTurn() - 1].getAction(state));
      }
    }
  }
  state.print();
  return state.getScore();
}


int main(int argc, char* argv[]) {
  assert(argc >= 2);
  string map = argv[1];

  vector<double> weights(kInitialWeights, kInitialWeights + sizeof(kInitialWeights) / sizeof(double));
  if (argc == 3) {
    string flag = argv[2];
    if (flag == "--weights") {
      GeneticUtils::getWeights(weights, map);
      cout << "overall best weights" << endl;
      for (unsigned int i = 0; i < weights.size(); i++) {
        cout << weights[i] << ",";
      }
      cout << endl;
    }
  } else {
    GameState g;
    g.load(map);
    vector<CatAgent> cats;
    for (int i = 1; i < g.getNumAgents(); i++) {
      cats.push_back(CatAgent(i));
    }
    MouseAgent mouse(weights, cats);
    run(g, mouse, cats);
  }
  return 0;
}
