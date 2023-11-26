#pragma once

#include "fades/fade.h"

class HorizontalWashFade : public Fade
{
public:
  HorizontalWashFade(int w) : Fade(w) {};
  void run() override;
};
