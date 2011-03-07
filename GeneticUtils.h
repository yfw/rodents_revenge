#ifndef GENETICUTILS_H
#define GENETICUTILS_H

#include "GameState.h"
#include "Agents.h"

using namespace __gnu_cxx;

class GeneticUtils {
 public:
  static void getWeights(vector<double>& weights, string map);
   
 private:
  static int run(const GameState& state,
	        const MouseAgent& mouse,
	        const vector<CatAgent>& cats);

  static double unifRand();
  static double unifRand(double a, double b);
  static double getNoise(double noiseSize, int populationIndex);
  static double getNoise(double noiseSize);
};

#endif

