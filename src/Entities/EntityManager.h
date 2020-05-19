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
    EntityManager(Worlds::WorldManager& worldManager, Player& player);

    void StoreEntity(Worlds::Room& room, Entity& entity);

    /**
     * @brief Perform behavior for all entities in the current room
     */
    inline void CycleEntitiesInCurrentRoom()
    {
        CycleEntitiesInRoom(m_WorldManager.GetCurrentRoom());
    }

    bool TryMovePlayerEntity(Direction dir);

    const Entity* GetApproachedEntity(const Character& character) const;

private:
    Worlds::WorldManager& m_WorldManager;
    Player& m_Player;
    std::unordered_map<Worlds::Room*, std::vector<std::unique_ptr<Entities::Entity>>> m_EntityStorage;

    bool CanCharacterMove(const Character& character, Direction dir) const;

    const std::array<const Worlds::Field*, 4> GetFieldsNextToEntity(const Entity& entity) const;

    const Worlds::Field* GetFieldNextToEntity(const Entity& entity, Direction dir) const;

    void CycleEntitiesInRoom(Worlds::Room& room);

    void PlaceEntityInRoom(Entity& entity, Worlds::Room& room);

    void VacateEntityFieldInRoom(Entity& entity, Worlds::Room& room);
};

} /* namespace Entities */
