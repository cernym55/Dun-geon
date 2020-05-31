#include "Field.h"
#include "Entities/Entity.h"
#include "Misc/Coords.h"
#include "Misc/Exceptions.h"
#include <memory>
#include <sstream>

namespace Worlds
{

Field::Field(Coords coords)
    : m_Coords(coords),
      m_ForegroundEntity(nullptr),
      m_BackgroundEntity(nullptr),
      m_Accessible(false)
{
}

Coords Field::GetCoords() const
{
    return m_Coords;
}

const Entities::Entity* Field::ForegroundEntity() const
{
    return m_ForegroundEntity;
}

const Entities::Entity* Field::BackgroundEntity() const
{
    return m_BackgroundEntity;
}

void Field::PlaceEntity(Entities::Entity& entity)
{
    if (entity.IsBlocking() && m_ForegroundEntity == nullptr)
    {
        m_ForegroundEntity = &entity;
    }
    else if (!entity.IsBlocking() && m_BackgroundEntity == nullptr)
    {
        m_BackgroundEntity = &entity;
    }
    else
    {
        std::ostringstream errorMessage;
        errorMessage << "Tried to place entity into overlap at: "
                     << m_Coords;
        throw InvalidPositionException(errorMessage.str());
    }
}

Entities::Entity* Field::VacateForeground()
{
    Entities::Entity* tmp = m_ForegroundEntity;
    m_ForegroundEntity = nullptr;
    return tmp;
}

Entities::Entity* Field::VacateBackground()
{
    Entities::Entity* tmp = m_BackgroundEntity;
    m_BackgroundEntity = nullptr;
    return tmp;
}

bool Field::IsAccessible() const
{
    return m_Accessible;
}

void Field::MakeAccessible()
{
    m_Accessible = true;
}

} /* namespace Worlds */