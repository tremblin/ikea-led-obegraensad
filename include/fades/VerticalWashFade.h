#pragma once

#include "fades/fade.h"

class VerticalWashFade : public Fade
{
public:
  VerticalWashFade(int w) : Fade(w) {};
  void run() override;
};
