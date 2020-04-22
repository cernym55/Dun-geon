#include "Direction.h"
#include <ostream>

Direction::Direction()
    : m_Value(Value::Up)
{
}

Direction::Direction(Value value)
    : m_Value(value)
{
}

bool Direction::operator==(const Direction& r) const
{
    return m_Value == r.m_Value;
}

bool Direction::operator!=(const Direction& r) const
{
    return !(*this == r);
}

Direction::Value Direction::operator()() const
{
    return m_Value;
}

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

int Direction::ToInt() const
{
    return static_cast<int>(m_Value);
}

Direction Direction::Up()
{
    return Direction(Value::Up);
}

Direction Direction::Right()
{
    return Direction(Value::Right);
}

Direction Direction::Down()
{
    return Direction(Value::Down);
}

Direction Direction::Left()
{
    return Direction(Value::Left);
}

Direction Direction::None()
{
    return Direction(Value::None);
}

std::array<Direction, 4> Direction::All()
{
    return { Up(), Right(), Down(), Left() };
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