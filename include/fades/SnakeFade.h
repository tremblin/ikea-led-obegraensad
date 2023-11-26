#pragma once

#include "fades/fade.h"

class SnakeFade : public Fade
{
public:
  SnakeFade(int w) : Fade(w) {};
  void run() override;
};
