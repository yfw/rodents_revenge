
#include "Renderer.h"

Renderer::Renderer() {
  imgMap_[BLOCK].LoadFromFile("Sprites/block.png");
  imgMap_[WALL].LoadFromFile("Sprites/wall.png");
  imgMap_[CAT].LoadFromFile("Sprites/cat.png");
  imgMap_[MOUSE].LoadFromFile("Sprites/mouse.png");
  imgMap_[CHEESE].LoadFromFile("Sprites/cheese.png");
  imgMap_[NOTHING].LoadFromFile("Sprites/nothing.png");

  spriteMap_[BLOCK].SetImage(imgMap_[BLOCK]);
  spriteMap_[WALL].SetImage(imgMap_[WALL]);
  spriteMap_[CAT].SetImage(imgMap_[CAT]);
  spriteMap_[MOUSE].SetImage(imgMap_[MOUSE]);
  spriteMap_[CHEESE].SetImage(imgMap_[CHEESE]);
  spriteMap_[NOTHING].SetImage(imgMap_[NOTHING]);
}

void Renderer::render(
  sf::RenderWindow& app,
  const GameState& state) {

  app.Clear();
  for (int y = 0; y < kLevelRows; y++) {
    for (int x = 0; x < kLevelCols; x++) {
      ObjType type = state.get(x, y);
      spriteMap_[type].SetPosition(x * 16, y * 16);
      app.Draw(spriteMap_[type]);
    }
  }
  app.Display();
}
