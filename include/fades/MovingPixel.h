#pragma once

#include <Arduino.h>
#include <vector>

class MovingPixel
{
private:
  std::vector<float> position;
  std::vector<float> speed;
  std::vector<float> acceleration;
  float delay;
  uint8_t brightness;
  time_t start;

  void setPixel(uint8_t brightness);

public:
  MovingPixel(std::vector<float> p, std::vector<float> s = {}, std::vector<float> a = {}, uint8_t b = 255, float d = 0) : position(p), speed(s), acceleration(a), brightness(b), delay(d)
  {
    time(&start);
  }
  void clear();
  void draw();
  bool step();
};
