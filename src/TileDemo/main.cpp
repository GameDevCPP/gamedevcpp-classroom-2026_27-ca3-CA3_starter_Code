#include "engine.h"
#include "scenes/scene_tile_demo.h"

TileDemoScene tileDemo;

int main() {
  Engine::Start(900, 650, "Tile Demo", &tileDemo);
  return 0;
}
