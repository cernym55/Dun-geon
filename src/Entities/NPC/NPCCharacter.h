#pragma once

#include "Character.h"
#include "CharacterGenerationParameters.h"

namespace Entities::NPC
{

/**
 * @brief Character with randomizable stats
 */
class NPCCharacter : public Character
{
public:
    /**
     * @brief Constructor
     *
     * @param parameters parameters
     * @param name name
     * @param description description (default: empty)
     * @param icon icon (default: set to first character of name)
     * @param initialStats initial stats (default: arbitrary values)
     * @param isBlocking blocking attribute (default: true)
     */
    NPCCharacter(const CharacterGenerationParameters& parameters,
                 const std::string& name,
                 const std::string& description = "",
                 chtype icon = 0,
                 Stats initialStats = { 1, 10, 10, 5, 10, 5, 5, 5, 10 },
                 bool isBlocking = true);
    
    /**
     * @brief Destructor
     */
    virtual ~NPCCharacter() = default;
};

} /* namespace Entities::NPC */