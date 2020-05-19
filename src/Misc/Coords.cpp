#include "Coords.h"
#include "Direction.h"
#include "Entities/Character.h"
#include "Exceptions.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <vector>

Coords::Coords()
    : m_XCoord(0), m_YCoord(0)
{
}

Coords::Coords(Scalar xCoord, Scalar yCoord)
    : m_XCoord(xCoord), m_YCoord(yCoord)
{
}

bool Coords::operator==(const Coords& r) const
{
    return m_XCoord == r.m_XCoord && m_YCoord == r.m_YCoord;
}

Coords::Scalar Coords::GetX() const
{
    return m_XCoord;
}

void Coords::SetX(Scalar value)
{
    m_XCoord = value;
}

Coords::Scalar Coords::GetY() const
{
    return m_YCoord;
}

void Coords::SetY(Scalar value)
{
    m_YCoord = value;
}

Coords Coords::GetAdjacent(Direction dir) const
{
    Coords adjacentCoords(*this);
    switch (dir())
    {
    case Direction::Value::Up:
        adjacentCoords.m_YCoord--;
        break;
    case Direction::Value::Right:
        adjacentCoords.m_XCoord++;
        break;
    case Direction::Value::Down:
        adjacentCoords.m_YCoord++;
        break;
    case Direction::Value::Left:
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

bool Coords::SharesAxisWith(Coords there) const
{
    return m_XCoord == there.m_XCoord || m_YCoord == there.m_YCoord;
}

std::vector<Coords> Coords::StraightPathTo(Coords there) const
{
    if (!SharesAxisWith(there))
    {
        std::ostringstream errorMessage;
        errorMessage
            << "No straight path exists from "
            << *this
            << " to "
            << there;
        throw InvalidPositionException(errorMessage.str());
    }

    std::vector<Coords> path;
    if (m_XCoord == there.m_XCoord)
    {
        for (Scalar y = m_YCoord; y != there.m_YCoord; m_YCoord < there.m_YCoord ? y++ : y--)
        {
            path.push_back({ m_XCoord, y });
        }
    }
    else if (m_YCoord == there.m_YCoord)
    {
        for (Scalar x = m_XCoord; x != there.m_XCoord; m_XCoord < there.m_XCoord ? x++ : x--)
        {
            path.push_back({ x, m_YCoord });
        }
    }
    path.push_back(there);

    return path;
}

Coords::Scalar Coords::CombinedDistanceFrom(Coords there) const
{
    return Abs(m_XCoord - there.m_XCoord) + Abs(m_YCoord - there.m_YCoord);
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