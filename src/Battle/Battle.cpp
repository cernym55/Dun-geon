#include "Battle.h"
#include "Misc/RNG.h"
#include "Skill.h"
#include "SkillCollection.h"
#include "UI/BattleScreen.h"
#include <sstream>

namespace Battle
{

Battle::Battle(Entities::Player& player, Entities::Character& enemy)
    : m_Player(player),
      m_Enemy(enemy),
      m_BattleScreen(nullptr),
      m_Result(Result::Ongoing),
      m_PlayerProfile(m_Player.GetStats()),
      m_EnemyProfile(m_Enemy.GetStats())
{
}

void Battle::SetBattleScreen(UI::BattleScreen* battleScreen)
{
    m_BattleScreen = battleScreen;
}

const Entities::Player& Battle::GetPlayer() const
{
    return m_Player;
}

const Entities::Character& Battle::GetEnemy() const
{
    return m_Enemy;
}

Battle::Result Battle::DoBattle()
{
    bool playerTurn = true;
    while (m_Result == Result::Ongoing)
    {
        if (playerTurn)
        {
            DoPlayerTurn();
        }
        else
        {
            DoEnemyTurn();
        }
        playerTurn = !playerTurn;
    }

    FinishBattle();
    return m_Result;
}

const BattleProfile& Battle::GetPlayerProfile() const
{
    return m_PlayerProfile;
}

const BattleProfile& Battle::GetEnemyProfile() const
{
    return m_EnemyProfile;
}

void Battle::DoPlayerTurn()
{
    UpdateActiveEffects(m_PlayerProfile);
    m_BattleScreen->DisplayPlayerActiveEffects();
    m_BattleScreen->DisplayEnemyActiveEffects();

    if (m_PlayerProfile.Stats.Health <= 0)
    {
        m_Result = Result::GameOver;
        return;
    }

    // Filter available options
    static std::map<int, std::string> actions = { { 0, "Melee" }, { 1, "Ranged" }, { 2, "Spell" }, { 20, "Escape" } };
    std::map<int, Skill*> specialSkills;
    int specialSkillNumber = 10;
    bool hasMelee          = false;
    bool hasRanged         = false;
    bool hasSpell          = false;
    for (auto& skill : m_Player.GetSkills())
    {
        if (skill->GetCategory() == Skill::Category::Melee)
            hasMelee = true;
        else if (skill->GetCategory() == Skill::Category::Ranged)
            hasRanged = true;
        else if (skill->GetCategory() == Skill::Category::Spell)
            hasSpell = true;
        else if (skill->GetCategory() == Skill::Category::Special)
        {
            actions[specialSkillNumber]       = skill->GetName();
            specialSkills[specialSkillNumber] = skill.get();
            specialSkillNumber++;
        }
    }
    if (!hasMelee)
        actions.erase(0);
    if (!hasRanged)
        actions.erase(1);
    if (!hasSpell)
        actions.erase(2);

ACTION_CHOICE:
    constexpr int RethinkCode = 1000;
    m_BattleScreen->PostMessage("What will " + m_Player.GetName() + " do?");

    int choice = m_BattleScreen->SelectPlayerAction(actions);
    switch (choice)
    {
    case 0:
    {
        m_BattleScreen->PostMessage("Which attack?");
        std::map<int, Skill*> availableSkills;
        std::map<int, std::string> options { { RethinkCode, "<rethink>" } };
        int counter = 0;
        for (auto& skill : m_Player.GetSkills())
        {
            if (skill->GetCategory() == Skill::Category::Melee)
            {
                availableSkills[counter] = skill.get();
                options[counter]         = skill->GetName();
                counter++;
            }
        }

        int meleeChoice = m_BattleScreen->SelectWithHoverAction(options, [&](auto it) {
            m_BattleScreen->ClearProjectionArea();
            m_BattleScreen->ClearThumbnailArea();
            if (it->first == RethinkCode)
                return;

            availableSkills.at(it->first)->OnBattleMenuHover(*m_BattleScreen);
        });

        if (meleeChoice == RethinkCode)
            goto ACTION_CHOICE;

        m_BattleScreen->PostMessage("");
        m_BattleScreen->ClearProjectionArea();
        m_BattleScreen->ClearThumbnailArea();
        LaunchAttack(*availableSkills.at(meleeChoice), true);

        break;
    }
    case 20:
        m_Result = Result::Escape;
        break;
    default:
        if (specialSkills.count(choice) > 0)
        {
            auto specialSkill = specialSkills.at(choice);
            m_BattleScreen->PostMessage(m_Player.GetName() + " uses " + specialSkills.at(choice)->GetName() + "!");
            switch (specialSkill->GetTargetType())
            {
            case Skill::Target::Opponent:
                LaunchAttack(*specialSkill, true);
                break;
            case Skill::Target::Self:
                LaunchAttack(*specialSkill, true);
                break;
            // TODO: Add option to choose target or target both
            default:
                break;
            }
        }
        break;
    }
}

void Battle::DoEnemyTurn()
{
    UpdateActiveEffects(m_EnemyProfile);
    m_BattleScreen->DisplayPlayerActiveEffects();
    m_BattleScreen->DisplayEnemyActiveEffects();

    if (m_EnemyProfile.Stats.Health <= 0)
    {
        m_Result = Result::Victory;
        return;
    }

    m_BattleScreen->PostMessage(m_Enemy.GetName() + " attacks!");
    std::map<int, Skill*> availableSkills;
    int counter = 0;
    for (auto& skill : m_Enemy.GetSkills())
    {
        availableSkills[counter] = skill.get();
        counter++;
    }

    // Pick random skill
    LaunchAttack(*availableSkills.at(RNG::RandomInt(availableSkills.size())), false);
}

void Battle::LaunchAttack(Skill& skill, bool isPlayer)
{
    if (isPlayer)
    {
        switch (skill.GetTargetType())
        {
        case Skill::Target::Opponent:
            skill.ApplySkill(m_PlayerProfile, m_EnemyProfile);
            break;
        case Skill::Target::Self:
            skill.ApplySkill(m_PlayerProfile, m_PlayerProfile);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (skill.GetTargetType())
        {
        case Skill::Target::Opponent:
            skill.ApplySkill(m_EnemyProfile, m_PlayerProfile);
            break;
        case Skill::Target::Self:
            skill.ApplySkill(m_EnemyProfile, m_EnemyProfile);
            break;
        default:
            break;
        }
    }

    skill.AnimateTo(*m_BattleScreen, isPlayer);
}

void Battle::FinishBattle()
{
    m_BattleScreen->BattleEndMessage(m_Result);

    // Update remaining player HP/MP
    m_Player.SetHealth(m_PlayerProfile.Stats.Health);
    m_Player.SetMana(m_PlayerProfile.Stats.Mana);
}

void Battle::UpdateActiveEffects(BattleProfile& profile)
{
    for (auto& effect : profile.ActiveEffects)
    {
        effect->Tick();
        if (effect->GetRemainingDuration() == 0)
        {
            effect->Remove();
        }
    }
    profile.ActiveEffects.erase(std::remove_if(profile.ActiveEffects.begin(),
                                               profile.ActiveEffects.end(),
                                               [](auto& effect) { return effect->GetRemainingDuration() == 0; }),
                                profile.ActiveEffects.end());
}

} /* namespace Battle */