#include "Coords.h"
#include "Direction.h"
#include "Entities/Character.h"
#include "Exceptions.h"
#include <iostream>

Coords::Coords()
    : m_XCoord(0), m_YCoord(0)
{
}

Coords::Coords(size_t xCoord, size_t yCoord)
    : m_XCoord(xCoord), m_YCoord(yCoord)
{
}

bool Coords::operator==(const Coords& r) const
{
    return m_XCoord == r.m_XCoord && m_YCoord == r.m_YCoord;
}

size_t Coords::GetX() const
{
    return m_XCoord;
}

void Coords::SetX(size_t value)
{
    m_XCoord = value;
}

size_t Coords::GetY() const
{
    return m_YCoord;
}

void Coords::SetY(size_t value)
{
    m_YCoord = value;
}

Coords Coords::GetAdjacent(Direction dir) const
{
    Coords adjacentCoords(*this);
    switch (dir())
    {
    case Direction::Value::Up:
        if (m_YCoord == 0) throw InvalidPositionException("Attempted to move Y coord below 0");
        adjacentCoords.m_YCoord--;
        break;
    case Direction::Value::Right:
        adjacentCoords.m_XCoord++;
        break;
    case Direction::Value::Down:
        adjacentCoords.m_YCoord++;
        break;
    case Direction::Value::Left:
        if (m_XCoord == 0) throw InvalidPositionException("Attempted to move X coord below 0");
        adjacentCoords.m_XCoord--;
        break;
    default:
        break;
    }

    return adjacentCoords;
}

Coords& Coords::MoveInDirection(Direction dir)
{
    Coords adjacentCoords = GetAdjacent(dir);
    m_XCoord = adjacentCoords.m_XCoord;
    m_YCoord = adjacentCoords.m_YCoord;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Coords& coords)
{
    return os
           << "["
           << static_cast<int>(coords.m_XCoord)
           << ", "
           << static_cast<int>(coords.m_YCoord)
           << "]";
}