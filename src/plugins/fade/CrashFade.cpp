#include <cmath>

#include "fades/CrashFade.h"

#include "screen.h"

#define START_BACKLOG 16

//#define SPEED(row) abs(float(ROWS - row) / 64)
#define SPEED_MAX 1
#define SPEED_FACTOR -0.10
#define SPEED(row) SPEED_MAX * exp(SPEED_FACTOR * (row + START_BACKLOG + 1))

void CrashFade::crash(int col, int row)
{
    uint8_t last = 0;
    uint8_t next = Screen.getPixel(col, row);

    if (next > 0)
    {
        while ((next > 0) && (row < (ROWS - 1)))
        {
            Screen.setPixel(col, row, last);
            last = next;
            next = Screen.getPixel(col, ++row);
        }
        if (row < ROWS)
            Screen.setPixel(col, row, last);
    }
}

void CrashFade::run()
{
    std::vector<float> position;

    //int start = random(0, 360);
    int start = 90;
    for (int i = 0; i < COLS; i++)
    {
        double deg = start + i * (double(360) / COLS);
        double rad = deg * PI / 180;
        position.push_back(float(START_BACKLOG / 2) * float(sin(rad)) - (START_BACKLOG / 2) - 1);
    }

    int finished = 0;
    while (finished < COLS)
    {
        for (int i = 0; i < COLS; i++)
        {
            int before = round(position[i]);
            position[i] += SPEED(position[i]);
            int after = round(position[i]);

            if ((after >= 0) && (after < ROWS) && (before != after))
            {
                crash(i, after);
            }
            
            if ((before < ROWS) && (after >= ROWS))
                finished++;
        }
        delay(20);
    }
}