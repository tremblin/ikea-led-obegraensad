#include "fades/SnakeFade.h"

#include "screen.h"

void SnakeFade::run()
{
    int lastX = -1, lastY = -1;
    for (int row = 0; row < ROWS; row++)
    {
        for (int i = 0; i < COLS; i++)
        {
            if ((lastX >= 0) && (lastY >= 0))
                Screen.setPixel(lastX, lastY, 0);

            int col = (row % 2) ? (COLS - i - 1) : i;
            Screen.setPixel(col, row, 1, 128);
            lastX = col;
            lastY = row;
            delay(20);
        }
    }
}