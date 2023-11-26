#include "fades/HorizontalWashFade.h"

#include "screen.h"

void HorizontalWashFade::run()
{
    for (int row = 0; row <= ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (row > 0)
                Screen.setPixel(col, row - 1, 0);
            if (row < ROWS)
                Screen.setPixel(col, row, 1, 128);
        }
        delay(40);
    }
}