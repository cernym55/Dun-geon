#include "BattleProfile.h"

namespace Battle
{

BattleProfile::BattleProfile(const Entities::Stats& stats) : Stats(stats), Resistances { 0 }
{
}

} /* namespace Battle */