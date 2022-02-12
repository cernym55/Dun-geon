#include "ApplyEffectOnlySkill.h"
#include "EffectCollection.h"
#include "UI/BattleScreen.h"

namespace Battle
{

template<typename EffectType> void ApplyEffectOnlySkill<EffectType>::OnBattleMenuHover(UI::BattleScreen& battleScreen)
{
    battleScreen.PrintSkillHoverThumbnailInfo(*this);
}

template<typename EffectType>
void ApplyEffectOnlySkill<EffectType>::AnimateTo(UI::BattleScreen& battleScreen, bool isPlayer) const
{
    if (!m_LastApplyResult)
    {
        throw CustomException("ApplyEffectOnlySkill::AnimateTo() failed - no last usage record");
    }

    if (isPlayer)
    {
        battleScreen.AnimatePlayerAttack(*m_LastApplyResult);
    }
    else
    {
        battleScreen.AnimateEnemyAttack(*m_LastApplyResult, m_Name);
    }
}

// Template declarations
template class ApplyEffectOnlySkill<EffectCollection::Brace>;

} /* namespace Battle */