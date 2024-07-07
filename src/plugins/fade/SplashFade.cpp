#include "fades/SplashFade.h"
#include "fades/MovingPixel.h"

#include "screen.h"

void SplashFade::run()
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
                std::vector<float> v = { (float((COLS - 1)/ 2) - x) / float(COLS) * 2, (float((ROWS - 1)/ 2) - y) / float(ROWS) * 2 };
                //std::vector<float> v = { (x - float((COLS - 1)/ 2)) / float(COLS) * 2, (y - float((ROWS - 1)/ 2)) / float(ROWS) * 2 };
                std::vector<float> acceleration = { 0, 0 };
                pixels.push_back(MovingPixel({ float(x), float(y) }, v, acceleration, brightness));
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