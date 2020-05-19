#include "StaticEntities.h"
#include "Entity.h"
#include "UI/ColorPairs.h"
#include <ncurses.h>

namespace Entities
{

/**
 * @brief Used to fill wall fields on the map
 */
Entity Wall("Wall", "", ' ' | COLOR_PAIR(UI::ColorPairs::BlackOnWhite));

} /* namespace Entities */
