#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "GeneticUtils.h"
#include "GameState.h"
#include "Agents.h"
#include "Constants.h"

int GeneticUtils::run(const GameState& state,
	const MouseAgent& mouse,
	const vector<CatAgent>& cats) {
  GameState g = state;
  while (!g.gameOver()) {
    if (g.getTurn() == 0) {
      g = g.getNext(mouse.getAction(g));
    } else {
      g = g.getNext(cats[g.getTurn() - 1].getAction(g));
    }
  }
  return g.getScore();
}

double GeneticUtils::unifRand() {
  return rand() / double(RAND_MAX);
}

double GeneticUtils::unifRand(double a, double b) {
  return (b-a) * unifRand() + a;
}

double GeneticUtils::getNoise(double noiseSize, int populationIndex) {
  if (populationIndex == 0) {
    return 0; // no noise for the first one
  } else {
    return unifRand(-1 * noiseSize, noiseSize);
  }
}

void GeneticUtils::getWeights(vector<double>& weights, string map) {
  const int populationSize = 3;
  const int numberOfGenerations = 2;
  const unsigned int numWeights = sizeof(kInitialWeights) / sizeof(double); 
  const double noiseSizes[] = {15, 0.1, 1, 5, 50, 0.1};
  srand(1);

  // initialize weights
  vector<vector<double> > population(populationSize, vector<double>());
  for (unsigned int i = 0; i < population.size(); i++) {
    for (unsigned int j = 0; j < numWeights; j++) {
      population[i].push_back(kInitialWeights[j] + getNoise(noiseSizes[j], i));
    }
  }
  
  double overallBestScore = 0;
  vector<double> overallBestWeights;

  // run with weights
  // when to terminate?
  for (int num = 0; num < numberOfGenerations; num++)  {
    for (unsigned int i = 0; i < population.size(); i++) {
      cout << "index=" << i << ":";
      for (unsigned int j = 0; j < numWeights; j++) {
        cout << population[i][j] << ",";
      }
      cout << endl;
    }
    vector<int> parentPool; 
    for (unsigned int i = 0; i < population.size(); i++) {
      GameState g;
      g.load(map);
      // TODO: randomize initial cat position?
      vector<CatAgent> cats;
      for (int catIdx = 1; catIdx < g.getNumAgents(); catIdx++) {
        cats.push_back(CatAgent(catIdx));
      }
      MouseAgent mouse(population[i], cats);
      time_t seconds = time(NULL);
      srand(1);
      int score = run(g, mouse, cats);
      if (score > overallBestScore) {
        overallBestScore = score;
        overallBestWeights = population[i];
      }
      for (int scoreCount = 0; scoreCount < score; scoreCount++) {
        parentPool.push_back(i);
      }
      parentPool.push_back(i); // so even the 0 score guys have a chance
      cout << "took " << time(NULL) - seconds << " seconds to run" << endl;
      cout << "score for chromosome " << i << " is " << score << endl;
    }

    // get new population
    vector<vector<double> > newPopulation(populationSize, vector<double>());
    for (unsigned int i = 0; i < newPopulation.size(); i++) {
      // mating with self is ok?
      int momIndex = parentPool[rand() % parentPool.size()];
      int dadIndex = parentPool[rand() % parentPool.size()];

      vector<double> momWeights = population[momIndex];
      vector<double> dadWeights = population[dadIndex];

      // procreate
      for (unsigned int weightIdx = 0; weightIdx < numWeights; weightIdx++) {
        if (rand() % 2) {
          newPopulation[i].push_back(momWeights[weightIdx]);
        } else {
          newPopulation[i].push_back(dadWeights[weightIdx]);
        }
        //TODO: mutation prob
      }
    }
    population = newPopulation;
  }
  weights = overallBestWeights;
}



