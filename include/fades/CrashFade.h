#pragma once

#include "fades/fade.h"

class CrashFade : public Fade
{
public:
  CrashFade(int w) : Fade(w) {};
  void run() override;
  void crash(int col, int row);
};
