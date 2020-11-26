#include "NPCCharacter.h"

namespace Entities::NPC
{

NPCCharacter::NPCCharacter(const CharacterGenerationParameters& parameters,
                           const std::string& name,
                           const std::string& description,
                           chtype icon,
                           Stats initialStats,
                           bool isBlocking)
    : Character(name, description, icon, initialStats, isBlocking)
{
}

} /* namespace Entities::NPC */