#include "Human.h"

namespace Entities::NPC
{

Human::Human(const std::string& name)
    : NPCCharacter({}, name, "Human")
{
}

} /* namespace Entities::NPC */