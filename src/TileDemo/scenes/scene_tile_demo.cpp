#include "scene_tile_demo.h"
#include "../components/cmp_drone.h"
#include "../components/cmp_sprite.h"
#include <LevelSystem.h>
#include <SFML/Graphics/Rect.hpp>
#include <memory>

using namespace sf;

namespace {
struct TileMapping {
  char tile;
  int col; // 32px column in res/tileset.png: wall, ground, platform, enemy, drone
};
const TileMapping kTiles[] = {
    {'w', 0}, {'g', 1}, {'p', 2}, {'n', 3}, {'d', 4},
};

IntRect rectFor(char tile) {
  for (auto& t : kTiles) {
    if (t.tile == tile) return IntRect(t.col * 32, 0, 32, 32);
  }
  return IntRect(0, 0, 32, 32);
}
} // namespace

void TileDemoScene::Load() {
  // Loads the same res/my_level.txt that TileEditor saves to. Edit the
  // level there (Save button), then rerun this to play the edit.
  ls::loadLevelFile("res/my_level.txt", 32.f);

  // 'n' (enemy) and 'd' (drone) are spawn markers, not permanent floor
  // tiles -- same convention Pacman/Platformer use for 's'/'e'/'n'.
  // LevelSystem only stores one character per cell, so a marker tile has
  // no floor underneath it until we put one back: read each marker's
  // position, reset that cell to 'g', then spawn a real Entity there.
  auto enemySpots = ls::findTiles('n');
  for (auto& pos : enemySpots) {
    ls::setTile(pos, 'g');
  }
  auto droneSpots = ls::findTiles('d');
  Vector2f droneSpawn = ls::getTilePosition({2, 2}); // fallback if none painted
  for (auto& pos : droneSpots) {
    droneSpawn = ls::getTilePosition(pos);
    ls::setTile(pos, 'g');
  }

  ls::setTextureMap("res/tileset.png");
  for (auto& t : kTiles) {
    ls::setTileRect(t.tile, IntRect(t.col * 32, 0, 32, 32));
  }

  // Spawned entities reuse the same tileset icons via SpriteComponent, so
  // an enemy marker looks identical standing on floor as it did painted
  // in the editor.
  _tileset = std::make_shared<sf::Texture>();
  _tileset->loadFromFile("res/tileset.png");

  for (auto& pos : enemySpots) {
    auto enemy = makeEntity();
    enemy->setPosition(ls::getTilePosition(pos));
    auto sprite = enemy->addComponent<SpriteComponent>();
    sprite->setTexure(_tileset);
    sprite->getSprite().setTextureRect(rectFor('n'));
  }

  auto drone = makeEntity();
  drone->setPosition(droneSpawn);
  drone->addComponent<DroneComponent>();

  setLoaded(true);
}

void TileDemoScene::UnLoad() {
  ls::unload();
  Scene::UnLoad();
}

void TileDemoScene::Update(const double& dt) { Scene::Update(dt); }

void TileDemoScene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
