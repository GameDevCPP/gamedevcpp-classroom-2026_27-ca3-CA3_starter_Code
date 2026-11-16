#pragma once
#include "engine.h"

class EditorScene : public Scene {
public:
  void Load() override;
  void Update(const double& dt) override;
  void Render() override;

private:
  char _brush = 'w';
};
