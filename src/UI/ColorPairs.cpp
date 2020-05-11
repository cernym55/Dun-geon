#include "ColorPairs.h"
#include <ncurses.h>

namespace UI
{

namespace ColorPairs
{

void InitPairs()
{
    for (short bg = 0; bg < 8; bg++)
    {
        for (short fg = 0; fg < 8; fg++)
        {
            init_pair(bg * 10 + fg, fg, bg == 0 ? -1 : bg);
        }
    }
    init_pair(BlackOnDefault, COLOR_BLACK, -1);
}

} /* namespace ColorPairs */

} /* namespace UI */