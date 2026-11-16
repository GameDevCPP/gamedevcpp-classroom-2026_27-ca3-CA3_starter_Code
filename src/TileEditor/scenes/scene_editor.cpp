#include "scene_editor.h"
#include <LevelSystem.h>
#include <imgui.h>
#include <iostream>

using namespace sf;

namespace {
// Tile char -> atlas column (each tile is 32x32, 6 tiles in a row).
struct Brush { char tile; const char* label; int col; };
const Brush kBrushes[] = {
    {'w', "Wall", 0}, {'g', "Ground", 1}, {'p', "Platform", 2},
    {'n', "Enemy", 3}, {'d', "Drone", 4},
};
} // namespace

void EditorScene::Load() {
  ls::newLevel(20, 15, ' ', 32.f);
  ls::setTextureMap("res/tileset.png");
  for (auto& b : kBrushes) {
    ls::setTileRect(b.tile, IntRect(b.col * 32, 0, 32, 32));
  }
  // Fallback flat colours, in case the texture ever fails to load.
  ls::setColor('w', Color(107, 107, 107));
  ls::setColor('g', Color(90, 143, 60));
  ls::setColor('p', Color(138, 90, 46));
  ls::setColor('n', Color::Red);
  ls::setColor('d', Color(59, 59, 59));
  setLoaded(true);
}

void EditorScene::Update(const double& dt) {
  Scene::Update(dt);

  ImGui::Begin("Tile editor");
  ImGui::Text("Click/drag on the grid to paint.");
  ImGui::Separator();
  for (auto& b : kBrushes) {
    if (ImGui::RadioButton(b.label, _brush == b.tile)) _brush = b.tile;
  }
  if (ImGui::RadioButton("Erase", _brush == ' ')) _brush = ' ';
  ImGui::Separator();
  if (ImGui::Button("Save res/my_level.txt")) {
    ls::saveLevelFile("res/my_level.txt");
    std::cout << "[editor] saved res/my_level.txt\n";
  }
  ImGui::SameLine();
  if (ImGui::Button("Load res/my_level.txt")) {
    ls::loadLevelFile("res/my_level.txt", 32.f);
    std::cout << "[editor] loaded res/my_level.txt\n";
  }
  ImGui::End();

  // Skip while ImGui wants the click (a palette button, say) so it
  // doesn't also paint the tile underneath.
  if (!ImGui::GetIO().WantCaptureMouse &&
      Mouse::isButtonPressed(Mouse::Left)) {
    auto mouse = Vector2f(Mouse::getPosition(Engine::GetWindow()));
    if (ls::isOnGrid(mouse)) {
      auto rel = (mouse - ls::getOffset()) / ls::getTileSize();
      ls::setTile(Vector2ul((size_t)rel.x, (size_t)rel.y), (ls::Tile)_brush);
    }
  }
}

void EditorScene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
