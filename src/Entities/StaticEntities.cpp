#include "StaticEntities.h"
#include "Entity.h"
#include "UI/ColorPairs.h"
#include <ncurses.h>

namespace Entities
{

Entity Wall("Wall", "", ' ' | COLOR_PAIR(UI::ColorPairs::Wall));

} /* namespace Entities */