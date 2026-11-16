#pragma once
#include <engine.h>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

class TileDemoScene : public Scene {
public:
  void Load() override;
  void UnLoad() override;
  void Update(const double& dt) override;
  void Render() override;

private:
  std::shared_ptr<sf::Texture> _tileset;
};
