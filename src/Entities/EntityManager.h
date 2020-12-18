#pragma once

#include "Character.h"
#include "Entity.h"
#include "Misc/Direction.h"
#include "NPC/NPCGenerator.h"
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
     * @brief Spawns a random entity
     *
     * @param room room
     * @param spawnPosition spawn position
     */
    void SpawnEntity(Worlds::Room& room, Coords spawnPosition);

    /**
     * @brief Kill the given (managed) entity, removing it from the world
     * 
     * @param entity entity to kill
     */
    void KillEntity(Entity& entity);

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
        Cycle(m_WorldManager.CurrentRoom());
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
     * @brief Get any entity the given entity is touching in the given direction
     *
     * @param entity entity
     * @param dir direction
     * @return Entity* entity being approached in direction
     */
    Entity* Approaching(const Entity& entity, Direction dir);

    /**
     * @brief Get any entity the given entity is touching in the given direction
     *
     * @param entity entity
     * @param dir direction
     * @return const Entity* entity being approached in direction
     */
    const Entity* Approaching(const Entity& entity, Direction dir) const;

    /**
     * @brief Check if the entity can move in the given direction
     *
     * @param entity entity
     * @param dir direction
     * @return true if can move
     */
    bool CanEntityMove(const Entity& entity, Direction dir) const;

    /**
     * @brief Get the entity's coords
     *
     * @param entity entity
     * @return Coords entity coords
     */
    Coords CoordsOf(const Entity& entity) const;

    /**
     * @brief Get the room the given entity is in
     * 
     * @param entity entity
     * @return const Worlds::Room& entity's room
     */
    const Worlds::Room& RoomOf(const Entity& entity) const;

private:
    Worlds::WorldManager& m_WorldManager;
    Player& m_Player;
    NPC::NPCGenerator m_NPCGenerator;
    std::unordered_map<const Worlds::Room*, std::vector<std::unique_ptr<Entity>>> m_EntityStorage;
    std::unordered_map<const Entity*, Worlds::Room*> m_RoomsByEntity;
    std::unordered_map<const Entity*, Coords> m_EntityCoords;

    /**
     * @brief Move the entity in the given direction
     *
     * @param entity entity
     * @param dir direction
     */
    void MoveEntity(Entity& entity, Direction dir);

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
