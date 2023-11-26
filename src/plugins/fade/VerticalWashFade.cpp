#include "fades/VerticalWashFade.h"

#include "screen.h"

void VerticalWashFade::run()
{
    for (int col = 0; col <= COLS; col++)
    {
        for (int row = 0; row < ROWS; row++)
        {
            if (col > 0)
                Screen.setPixel(col - 1, row, 0);
            if (col < COLS)
                Screen.setPixel(col, row, 1, 128);
        }
        delay(40);
    }
}