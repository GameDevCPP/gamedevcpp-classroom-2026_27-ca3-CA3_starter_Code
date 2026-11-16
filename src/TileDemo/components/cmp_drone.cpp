#include "cmp_drone.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>
#include <system_renderer.h>

using namespace sf;

DroneComponent::DroneComponent(Entity* p, float speed)
    : Component(p), _shape(14.f), _speed(speed) {
  _shape.setOrigin(14.f, 14.f);
  _shape.setFillColor(Color(70, 200, 220));
  _shape.setOutlineColor(Color::Black);
  _shape.setOutlineThickness(2.f);
}

bool DroneComponent::tryMove(const Vector2f& delta) {
  auto next = _parent->getPosition() + delta;
  if (!ls::isOnGrid(next) || ls::getTileAt(next) == ls::WALL) {
    return false;
  }
  _parent->setPosition(next);
  return true;
}

void DroneComponent::update(double dt) {
  Vector2f dir(0.f, 0.f);
  if (Keyboard::isKeyPressed(Keyboard::Left)) dir.x -= 1.f;
  if (Keyboard::isKeyPressed(Keyboard::Right)) dir.x += 1.f;
  if (Keyboard::isKeyPressed(Keyboard::Up)) dir.y -= 1.f;
  if (Keyboard::isKeyPressed(Keyboard::Down)) dir.y += 1.f;

  if (dir.x != 0.f || dir.y != 0.f) {
    dir = normalize(dir) * _speed * (float)dt;
    // Slide along walls: try the full move, then each axis on its own.
    if (!tryMove(dir)) {
      tryMove(Vector2f(dir.x, 0.f));
      tryMove(Vector2f(0.f, dir.y));
    }
  }
  _shape.setPosition(_parent->getPosition());
}

void DroneComponent::render() { Renderer::queue(&_shape); }
