#include "engine.h"
#include "scenes/scene_editor.h"

EditorScene editor;

int main() {
  Engine::Start(900, 650, "Tile Editor", &editor);
  return 0;
}
