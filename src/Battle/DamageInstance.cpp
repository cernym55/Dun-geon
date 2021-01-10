#include "DamageInstance.h"

namespace Battle
{

DamageInstance::DamageInstance(int value, DamageType damageType) : m_Value(value), m_DamageType(damageType)
{
}

void DamageInstance::PrintInColor(WINDOW* window) const
{
    wattron(window, COLOR_PAIR(m_DamageType.TextColor()) | A_BOLD);
    wprintw(window, "%d", m_Value);
    wattroff(window, A_COLOR | A_BOLD);
}

} /* namespace Battle */