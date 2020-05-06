#pragma once

#include "Entities/Entity.h"
#include "Misc/Coords.h"
#include <memory>
#include <vector>

namespace Worlds
{

class Field
{
public:
    /**
     * @brief Constructor
     * 
     * @param coords coordinates
     */
    Field(Coords coords);

    /**
     * @brief Get the coordinates
     * 
     * @return Coords coordinates
     */
    Coords GetCoords() const;

    /**
     * @brief Get the foreground entity
     * 
     * @return const Entities::Entity* foreground entity
     */
    const Entities::Entity* TryGetForegroundEntity() const;

    /**
     * @brief Get the background entity
     * 
     * @return const Entities::Entity& background entity
     */
    const Entities::Entity* TryGetBackgroundEntity() const;

    /**
     * @brief Place the given entity onto this field
     * If entity is blocking, use the foreground. If entity is not blocking, use the background.
     * 
     * @param entity entity
     */
    void PlaceEntity(Entities::Entity& entity);

    /**
     * @brief Pop any entity in the foreground or return null if vacant
     * 
     * @return Entities::Entity* evicted entity
     */
    Entities::Entity* VacateForeground();

    /**
     * @brief Pop any entity in the background or return null if vacant
     * 
     * @return Entities::Entity* evicted entity
     */
    Entities::Entity* VacateBackground();

    /**
     * @brief Check whether this field can be reached by the player
     * 
     * @return true if accessible
     */
    bool IsAccessible() const;

private:
    Coords m_Coords;
    Entities::Entity* m_ForegroundEntity;
    Entities::Entity* m_BackgroundEntity;
    bool m_Accessible;
    //std::vector<std::shared_ptr<Items::Item>> m_ContainedItems;
};

} /* namespace Worlds */