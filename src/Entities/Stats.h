#pragma once

namespace Entities
{

/**
 * @brief Maximum level normally attainable by characters
 */
constexpr static const int LevelCap = 100;

/**
 * @brief Collection of traditional RPG stats
 */
struct Stats
{
    int Level, Health, MaxHealth, Mana, MaxMana, Strength, Dexterity, Sorcery, Wisdom;
};

} /* namespace Entities */