
#ifndef CONSTANTS_H
#define CONSTANTS_H

static const unsigned int kLevelRows = 23;
static const unsigned int kLevelCols = 23;
static const int kMaxTime = 1000;
static const int kSpawnDistanceMin = 7;
static const double kInfinity = 10000000;
static const int kScoreEatCheese = 5;
static const int kScoreTrapCat = 1;
typedef char ObjType;
static const ObjType NOTHING = ' ';
static const ObjType WALL = '*';
static const ObjType BLOCK = '+';
static const ObjType MOUSE = 'M';
static const ObjType CAT = 'C';
static const ObjType CHEESE = 'H';
static const int kCats = 2;
static const double kInitialWeights[] = {40, 0.1, 1, 5, 100, 0.1, 400};
static const int kAlphaBetaDepth = 3;

#endif
