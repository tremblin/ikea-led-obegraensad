#pragma once

#include "fades/fade.h"

class NoneFade : public Fade
{
public:
  NoneFade(int w) : Fade(w) {}
};
