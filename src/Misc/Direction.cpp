#include "Direction.h"
#include <ostream>

Direction::Direction()
    : m_Value(Value::None)
{
}

Direction::Direction(Value value)
    : m_Value(value)
{
}

const Direction Direction::None = Direction(Direction::Value::None);

const Direction Direction::Up = Direction(Direction::Value::Up);

const Direction Direction::Right = Direction(Direction::Value::Right);

const Direction Direction::Down = Direction(Direction::Value::Down);

const Direction Direction::Left = Direction(Direction::Value::Left);

const std::array<Direction, 4> Direction::All = { Up, Right, Down, Left };

Direction Direction::Opposite() const
{
    switch (m_Value)
    {
    case Value::Up:
        return Down();
    case Value::Right:
        return Left();
    case Value::Down:
        return Up();
    case Value::Left:
        return Right();
    default:
        return None();
    }
}

std::ostream& operator<<(std::ostream& os, const Direction& r)
{
    switch (r())
    {
    case Direction::Value::Up:
        os << "up";
        break;
    case Direction::Value::Right:
        os << "right";
        break;
    case Direction::Value::Down:
        os << "down";
        break;
    case Direction::Value::Left:
        os << "left";
        break;
    case Direction::Value::None:
        os << "[none]";
        break;
    }

    return os;
}
