
#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Constants.h"

class Renderer {
 public:
  Renderer();
  void render(sf::RenderWindow& app, const GameState& state);	      

 private:
  map<ObjType, sf::Image> imgMap_;
  map<ObjType, sf::Sprite> spriteMap_;
};

#endif
