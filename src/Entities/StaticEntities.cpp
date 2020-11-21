#include "StaticEntities.h"
#include "Entity.h"
#include "UI/ColorPairs.h"
#include <ncurses.h>

namespace Entities
{

Entity Wall("Wall", "", ' ' | COLOR_PAIR(UI::ColorPairs::BlackOnWhite));

Entity Column("Column", "", 'I' | A_BOLD);

} /* namespace Entities */
