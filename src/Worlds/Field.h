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
     * @brief Constructor
     * 
     * @param r
     */
    Field(const Field& r);

    /**
     * @brief Default destructor
     */
    ~Field() = default;

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
    const Entities::Entity* GetForegroundEntity() const;

    /**
     * @brief Get the background entity
     * 
     * @return const Entities::Entity& background entity
     */
    const Entities::Entity* GetBackgroundEntity() const;

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

private:
    Coords m_Coords;
    Entities::Entity* m_ForegroundEntity;
    Entities::Entity* m_BackgroundEntity;
    //std::vector<std::shared_ptr<Items::Item>> m_ContainedItems;
};

} /* namespace Worlds */