#include "Coords.h"
#include "Direction.h"
#include "Entities/Character.h"
#include "Exceptions.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <vector>

Coords::Coords()
    : X(0), Y(0)
{
}

Coords::Coords(Scalar xCoord, Scalar yCoord)
    : X(xCoord), Y(yCoord)
{
}

Coords Coords::Adjacent(Direction dir) const
{
    Coords adjacentCoords(*this);
    switch (dir())
    {
    case Direction::Value::Up:
        adjacentCoords.Y--;
        break;
    case Direction::Value::Right:
        adjacentCoords.X++;
        break;
    case Direction::Value::Down:
        adjacentCoords.Y++;
        break;
    case Direction::Value::Left:
        adjacentCoords.X--;
        break;
    default:
        break;
    }

    return adjacentCoords;
}

Coords& Coords::Move(Direction dir)
{
    Coords adjacentCoords = Adjacent(dir);
    X = adjacentCoords.X;
    Y = adjacentCoords.Y;

    return *this;
}

bool Coords::SharesAxis(Coords there) const
{
    return X == there.X || Y == there.Y;
}

std::vector<Coords> Coords::StraightPath(Coords there) const
{
    if (!SharesAxis(there))
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
    if (X == there.X)
    {
        for (Scalar y = Y; y != there.Y; Y < there.Y ? y++ : y--)
        {
            path.push_back({ X, y });
        }
    }
    else if (Y == there.Y)
    {
        for (Scalar x = X; x != there.X; X < there.X ? x++ : x--)
        {
            path.push_back({ x, Y });
        }
    }
    path.push_back(there);

    return path;
}

Coords::Scalar Coords::Distance(Coords there) const
{
    return Abs(X - there.X) + Abs(Y - there.Y);
}

std::ostream& operator<<(std::ostream& os, const Coords& coords)
{
    return os
           << "["
           << static_cast<int>(coords.X)
           << ", "
           << static_cast<int>(coords.Y)
           << "]";
}
