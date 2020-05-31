#pragma once

#include "Character.h"
#include "Entity.h"
#include "Misc/Direction.h"
#include "Player.h"
#include "Worlds/Room.h"
#include "Worlds/WorldManager.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace Entities
{

/**
 * @brief Creates, stores and controls Entities and their behavior
 */
class EntityManager
{
public:
    /**
     * @brief Constructor
     *
     * @param WorldManager world manager
     * @param player player
     */
    EntityManager(Worlds::WorldManager& worldManager, Player& player);

    /**
     * @brief Take ownership of an entity and assign it to this room's storage
     *
     * @param room room
     * @param entity entity
     */
    void Store(Worlds::Room& room, Entity& entity);

    /**
     * @brief Perform behavior for all entities in the current room
     */
    inline void CycleCurrentRoom()
    {
        Cycle(m_WorldManager.GetCurrentRoom());
    }

    /**
     * @brief Try to move the player in the given direction
     * This counts as a player action and results in entities cycling if successful.
     *
     * @param dir direction
     * @return true if move successful
     */
    bool TryMovePlayer(Direction dir);

    /**
     * @brief Get the entity the character is touching
     * This is any entity next to the character in the direction of its last move.
     *
     * @param character character
     * @return const Entity* entity being approached
     */
    const Entity* Approaching(const Character& character) const;

private:
    Worlds::WorldManager& m_WorldManager;
    Player& m_Player;
    std::unordered_map<Worlds::Room*, std::vector<std::unique_ptr<Entities::Entity>>> m_EntityStorage;

    /**
     * @brief Check if the character can move in the given direction
     *
     * @param character character
     * @param dir direction
     * @return true if can move
     */
    bool CanCharacterMove(const Character& character, Direction dir) const;

    /**
     * @brief Get an array of fields surrounding the entity
     *
     * @param entity entity
     * @return const std::array<const Worlds::Field*, 4> surrounding fields
     */
    const std::array<const Worlds::Field*, 4> AdjacentFields(const Entity& entity) const;

    /**
     * @brief Get the field next to the entity in the given direction
     *
     * @param entity entity
     * @param direction direction
     * @return const Worlds::Field* neighboring field
     */
    const Worlds::Field* AdjacentField(const Entity& entity, Direction dir) const;

    /**
     * @brief Perform behavior for all entities in the given room
     *
     * @param room room
     */
    void Cycle(Worlds::Room& room);

    /**
     * @brief Place the entity in its position in the given room
     *
     * @param entity entity
     * @param room room
     */
    void Place(Entity& entity, Worlds::Room& room);

    /**
     * @brief Vacate the field occupied by the entity in the given room
     *
     * @param entity entity
     * @param room room
     */
    void Pluck(Entity& entity, Worlds::Room& room);
};

} /* namespace Entities */
