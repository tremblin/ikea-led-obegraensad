#include "fades/MovingPixel.h"

#include "screen.h"

#define ONSCREEN(x, y) ((x >= 0) && (x < COLS) && (y >= 0) && (y < ROWS))

bool MovingPixel::step()
{
    // start time reached?
    if (delay > 0)
    {
        uint64_t current = getMillis();
        float diff = (float(current) - float(start)) / 1000;

        if (delay > diff)
            return true;
    }

    // step
    int x = round(position[0]);
    int y = round(position[1]);
    if (ONSCREEN(x, y))
    {
        position[0] += speed[0];
        position[1] += speed[1];
        speed[0] += acceleration[0];
        speed[1] += acceleration[1];
        return true;
    }
    else
    {
        return false;
    }
}

void MovingPixel::setPixel(uint8_t brightness)
{
    int x = round(position[0]);
    int y = round(position[1]);
    if (ONSCREEN(x, y))
        Screen.setPixel(x, y, brightness);
}

void MovingPixel::clear()
{
    setPixel(0);
}

void MovingPixel::draw()
{
    setPixel(brightness);
}