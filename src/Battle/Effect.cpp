#include "Effect.h"
#include "BattleProfile.h"

namespace Battle
{

Effect::Effect(BattleProfile& user, BattleProfile& target, int duration)
    : OriginalDuration(duration),
      m_User(user),
      m_Target(target),
      m_RemainingDuration(duration)
{
}

void Effect::Tick()
{
    m_RemainingDuration--;
    TickAction();
}

void Effect::Refresh()
{
    m_RemainingDuration = OriginalDuration;
}

} /* namespace Battle */