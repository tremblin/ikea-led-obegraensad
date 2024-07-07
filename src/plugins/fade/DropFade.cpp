#include "fades/DropFade.h"
#include "fades/MovingPixel.h"

#include "screen.h"

#define MAX_V 32
#define MAX_A 0.3f
#define DELAY 0.3

void DropFade::run()
{
    std::vector<MovingPixel> pixels;
    uint8_t *rb = Screen.getRenderBuffer();
    for (int x = 0; x < COLS; x++)
    {
        for (int y = 0; y < ROWS; y++)
        {
            uint8_t brightness = rb[x + y * COLS];

            if (brightness > 0)
            {
                std::vector<float> v = { 0, float(random(1, MAX_V)) / MAX_V };
                std::vector<float> acceleration = { 0, float(random(1, MAX_V)) / MAX_V * MAX_A };
                float d = float(ROWS - y) * DELAY;
                pixels.push_back(MovingPixel({ float(x), float(y) }, v, acceleration, brightness, d));
            }
        }
    }

    bool visible = true;
    while (visible)
    {
        visible = false;
        for (MovingPixel &pixel : pixels)
        {
            pixel.clear();
            visible |= pixel.step();
        }
        for (MovingPixel &pixel : pixels)
        {
            pixel.draw();
        }
        delay(40);
    }
}