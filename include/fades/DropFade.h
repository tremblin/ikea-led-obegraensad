#pragma once

#include "fades/fade.h"

class DropFade : public Fade
{
public:
  DropFade(int w) : Fade(w) {};
  void run() override;
};
