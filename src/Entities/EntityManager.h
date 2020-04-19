#pragma once

#include "Entity.h"
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
     * @brief Delete copy constructor
     */
    EntityManager(const EntityManager&) = delete;

    /**
     * @brief Default destructor
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
    void UpdateEntitiesInCurrentRoom();

private:
    /**
     * @brief Perform behavior for all entities in the given room
     * 
     * @param room room
     */
    void UpdateEntitiesInRoom(Worlds::Room& room);

    /**
     * @brief Place the stored entities in the given room in their positions
     * 
     * @param room room
     */
    void PlaceStoredEntitiesInRoom(Worlds::Room& room);

    /**
     * @brief Vacate all fields occupied by stored entities in the given room
     * 
     * @param room room
     */
    void VacateStoredEntityFieldsInRoom(Worlds::Room& room);

    Worlds::WorldManager& m_WorldManager;
    Player& m_Player;
    std::unordered_map<Worlds::Room*, std::vector<std::unique_ptr<Entities::Entity>>> m_EntityStorage;
};

} /* namespace Entities */