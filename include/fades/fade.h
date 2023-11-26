#pragma once

class Fade
{
private:
  int weight;

public:
  Fade(int w) : weight(w) {}
  int getWeight()
  {
    return weight;
  }
  virtual void run() {}
};
