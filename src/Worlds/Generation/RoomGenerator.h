#pragma once

#include "Misc/Coords.h"
#include "RoomGenerationParameters.h"
#include "RoomLayout.h"
#include <memory>

namespace Worlds
{

class World;

} /* namespace Worlds */

namespace Worlds::Generation
{

/**
 * @brief Manages procedural generation of rooms in a single world
 */
class RoomGenerator
{
public:
    /**
     * @brief Constructor
     * 
     * @param world world
     */
    RoomGenerator(World& world);

    /**
     * @brief Create a layout of random type for the given coords
     * 
     * @param roomCoords room coords
     * @return std::unique_ptr<RoomLayout> new layout
     */
    std::unique_ptr<RoomLayout> CreateLayout(Coords roomCoords);

    /**
     * @brief Create a layout of specific type for the given coords
     * 
     * @param layoutType layout type
     * @param roomCoords room coords
     * @return std::unique_ptr<RoomLayout> new layout
     */
    std::unique_ptr<RoomLayout> CreateLayout(RoomLayout::Type layoutType, Coords roomCoords);

private:
    World& m_World;
    RoomGenerationParameters m_Parameters;
    int m_GeneratedRoomCount;
    int m_UndiscoveredRoomCount;

    /**
     * @brief Initialize the generation parameters struct
     */
    void InitializeParameters();

    /**
     * @brief Update the generation parameters struct
     */
    void UpdateParameters();

    /**
     * @brief Get the minimum number of rooms to generate
     * 
     * @return int room count floor
     */
    int RoomCountFloor() const;

    /**
     * @brief Get the maximum number of rooms to generate
     * 
     * @return int room count cap
     */
    int RoomCountCap() const;

    /**
     * @brief Get a map with info on which entrances a room at the given coords must or must not have.
     * A rule exists for every Direction key in the map.
     * A value of false means the entrance in this direction must not exist.
     * A value of false means the entrance in this direction must exist.
     * 
     * @param roomCoords room coords
     * @return std::map<Direction, bool> entrance info
     */
    std::map<Direction, bool> EntranceInfo(Coords roomCoords) const;
};

} /* namespace Worlds::Generation */
