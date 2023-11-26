#include "fades/RainFade.h"

#include "screen.h"

#define SPEED_STEPS 8
#define START_BACKLOG 8

void RainFade::run()
{
    std::vector<float> position;
    std::vector<float> speed;

    for (int i = 0; i < COLS; i++)
    {
        int start = random(0, START_BACKLOG);
        position.push_back(start - START_BACKLOG);
        speed.push_back(float(random(1, SPEED_STEPS)) / SPEED_STEPS);
    }

    int finished = 0;
    while (finished < COLS)
    {
        for (int i = 0; i < COLS; i++)
        {
            int before = round(position[i]);
            position[i] += speed[i];
            int after = round(position[i]);

            if ((before >= 0) && (before < ROWS) && (before != after))
                Screen.setPixel(i, before, 0);

            if ((after >= 0) && (after < ROWS))
                Screen.setPixel(i, after, 1, 128);
            
            if ((before < ROWS) && (after >= ROWS))
                finished++;
        }
        delay(40);
    }
}