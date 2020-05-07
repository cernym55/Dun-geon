#pragma once

#include "Misc/Direction.h"
#include "UI/CameraStyle.h"
#include <map>

namespace Worlds
{

namespace Generation
{

/**
 * @brief Contains parameters for room and room layout generation
 */
struct RoomGenerationParameters
{
    /**
     * @brief Contains info on which entrances must or must not be generated
     * A value of true for a direction means this entrance must be generated.
     * A value of false for a direction means this entrance must not be generated.
     * No value for a direction means the entrance may be generated optionally at random.
     */
    std::map<Direction, bool> EntranceInfo;

    /**
     * @brief Force this room to have at least two entrances
     */
    bool ForceContinue;

    /**
     * @brief Chance for optional entrances to appear
     */
    double OptionalEntranceChance;

    /**
     * @brief Chance for the room to be dark
     */
    double DarknessChance;
};

} /* namespace Generation */

} /* namespace Worlds */