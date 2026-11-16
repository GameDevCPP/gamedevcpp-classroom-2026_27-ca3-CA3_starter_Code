#pragma once
#include <ecm.h>
#include <SFML/Graphics/CircleShape.hpp>

// A keyboard-controlled circle that can't walk through WALL tiles.
class DroneComponent : public Component {
public:
  explicit DroneComponent(Entity* p, float speed = 150.f);
  DroneComponent() = delete;

  void update(double dt) override;
  void render() override;

private:
  sf::CircleShape _shape;
  float _speed;

  bool tryMove(const sf::Vector2f& delta);
};
