#include "DamageType.h"
#include "Misc/Exceptions.h"
#include "UI/ColorPairs.h"

namespace Battle
{

DamageType::DamageType(Value value) : m_Value(value)
{
}

const DamageType DamageType::Physical = DamageType(DamageType::Value::Physical);

const DamageType DamageType::Fire = DamageType(DamageType::Value::Fire);

const DamageType DamageType::Frost = DamageType(DamageType::Value::Frost);

const DamageType DamageType::Poison = DamageType(DamageType::Value::Poison);

const DamageType DamageType::Magic = DamageType(DamageType::Value::Magic);

const std::array<DamageType, 5> DamageType::All = { Physical, Fire, Frost, Poison, Magic };

std::string DamageType::ToString() const
{
    switch (m_Value)
    {
    case Value::Physical:
        return "Physical";
    case Value::Fire:
        return "Fire";
    case Value::Frost:
        return "Frost";
    case Value::Poison:
        return "Poison";
    case Value::Magic:
        return "Magic";
    default:
        throw InvalidEnumValueException("Attempted to convert invalid damage type value to string");
    }
}

short DamageType::TextColor() const
{
    switch (m_Value)
    {
    case Value::Physical:
        return UI::ColorPairs::WhiteOnDefault;
    case Value::Fire:
        return UI::ColorPairs::RedOnDefault;
    case Value::Frost:
        return UI::ColorPairs::BlueOnDefault;
    case Value::Poison:
        return UI::ColorPairs::GreenOnDefault;
    case Value::Magic:
        return UI::ColorPairs::MagentaOnDefault;
    default:
        throw InvalidEnumValueException("Attempted to get text color of invalid damage type");
    }
}

void DamageType::PrintInColor(WINDOW* window) const
{
    wattron(window, COLOR_PAIR(TextColor()) | A_BOLD);
    waddstr(window, ToString().c_str());
    wattroff(window, A_COLOR | A_BOLD);
}

} /* namespace Battle */