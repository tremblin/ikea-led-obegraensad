#pragma once

#include "fades/fade.h"

class SplashFade : public Fade
{
public:
  SplashFade(int w) : Fade(w) {};
  void run() override;
};
