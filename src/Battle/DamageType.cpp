#include "DamageType.h"
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
        throw;
    }
}

void DamageType::PrintColor(WINDOW* window) const
{
    short color;

    switch (m_Value)
    {
    case Value::Physical:
        color = UI::ColorPairs::WhiteOnDefault;
        break;
    case Value::Fire:
        color = UI::ColorPairs::RedOnDefault;
        break;
    case Value::Frost:
        color = UI::ColorPairs::BlueOnDefault;
        break;
    case Value::Poison:
        color = UI::ColorPairs::GreenOnDefault;
        break;
    case Value::Magic:
        color = UI::ColorPairs::MagentaOnDefault;
        break;
    }

    wattron(window, COLOR_PAIR(color) | A_BOLD);
    waddstr(window, ToString().c_str());
    wattroff(window, A_COLOR | A_BOLD);
}

} /* namespace Battle */