#include "Human.h"

namespace Entities::NPC
{

Human::Human(Coords initialCoords, const std::string& name)
    : NPCCharacter(initialCoords, {}, name, "Human")
{
}

} /* namespace Entities::NPC */