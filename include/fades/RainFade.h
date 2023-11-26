#pragma once

#include "fades/fade.h"

class RainFade : public Fade
{
public:
  RainFade(int w) : Fade(w) {};
  void run() override;
};
