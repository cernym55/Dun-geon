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

    EntityManager(const EntityManager&) = delete;

    EntityManager(EntityManager&&) = delete;

    EntityManager& operator=(const EntityManager&) = delete;

    EntityManager& operator=(EntityManager&&) = delete;

    /**
     * @brief Destructor
     */
    ~EntityManager() = default;

    /**
     * @brief Take ownership of an entity and assign it to this room's storage
     * 
     * @param room room
     * @param entity entity
     */
    void StoreEntity(Worlds::Room& room, Entity& entity);

    /**
     * @brief Perform behavior for all entities in the current room
     */
    void CycleEntitiesInCurrentRoom();

    /**
     * @brief Try to move the player in the given direction
     * This counts as a player action and results in entities cycling if successful.
     * 
     * @param dir direction
     * @return true if move successful
     */
    bool TryMovePlayerEntity(Direction dir);

    /**
     * @brief Get the entity the character is touching
     * This is any entity next to the character in the direction of its last move.
     * 
     * @param character character
     * @return const Entity* entity being approached
     */
    const Entity* GetApproachedEntity(const Character& character) const;

private:
    Worlds::WorldManager& m_WorldManager;
    Player& m_Player;
    std::unordered_map<Worlds::Room*, std::vector<std::unique_ptr<Entities::Entity>>> m_EntityStorage;

    /**
     * @brief Check if the character will leave the room if it moves in the given direction
     * 
     * @param character character
     * @param moveDirection direction of the move
     * @return true if player will leave the room
     */
    bool IsCharacterAboutToLeaveRoom(const Character& character,
                                     Direction moveDirection) const;

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
    const std::array<const Worlds::Field*, 4> GetFieldsNextToEntity(const Entity& entity) const;

    /**
     * @brief Get the field next to the entity in the given direction
     * 
     * @param entity entity
     * @param direction direction
     * @return const Worlds::Field* neighboring field
     */
    const Worlds::Field* GetFieldNextToEntity(const Entity& entity, Direction dir) const;

    /**
     * @brief Perform behavior for all entities in the given room
     * 
     * @param room room
     */
    void CycleEntitiesInRoom(Worlds::Room& room);

    /**
     * @brief Place the entity in its position in the given room
     * 
     * @param entity entity
     * @param room room
     */
    void PlaceEntityInRoom(Entity& entity, Worlds::Room& room);

    /**
     * @brief Vacate the field occupied by the entity in the given room
     * 
     * @param entity entity
     * @param room room
     */
    void VacateEntityFieldInRoom(Entity& entity, Worlds::Room& room);
};

} /* namespace Entities */