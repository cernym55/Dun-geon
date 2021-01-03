#include "BattleScreen.h"
#include "Components/FillBar.h"
#include <algorithm>
#include <chrono>
#include <ncurses.h>
#include <sstream>
#include <thread>

using BattleResult = Battle::Battle::Result;

namespace UI
{

BattleScreen::BattleScreen(Battle::Battle& battle, Screen& screen, InputHandler& inputHandler)
    : Subscreen(screen, inputHandler),
      m_Battle(battle),
      m_ArenaPanelWindow(nullptr),
      m_BottomPanelWindow(nullptr),
      m_StatPanelWindow(nullptr),
      m_PlayerNameplate(m_Battle.GetPlayer(),
                        (ArenaPanelWidth - ArenaNameplateWidth) / 2,
                        2 + Components::Nameplate::Height + 3,
                        ArenaNameplateWidth,
                        false),
      m_EnemyNameplate(m_Battle.GetEnemy(), (ArenaPanelWidth - ArenaNameplateWidth) / 2, 2, ArenaNameplateWidth, true),
      m_LogWindow(ArenaPanelWidth, 0, LogPanelWidth, TopPanelHeight)
{
    Init();
}

BattleScreen::~BattleScreen()
{
    Terminate();
}

void BattleScreen::Init()
{
    if (m_ArenaPanelWindow == nullptr)
        m_ArenaPanelWindow = newwin(
            Components::Nameplate::Height * 2 + 3, ArenaNameplateWidth, 2, (ArenaPanelWidth - ArenaNameplateWidth) / 2);
    if (m_BottomPanelWindow == nullptr)
        m_BottomPanelWindow = newwin(BottomPanelHeight, ArenaPanelWidth, TopPanelHeight, 0);
    if (m_StatPanelWindow == nullptr)
        m_StatPanelWindow = newwin(BottomPanelHeight, LogPanelWidth, TopPanelHeight, ArenaPanelWidth);

    m_Screen.Clear();

    AnimateBattleStart();
    DrawScreenLayout();
}

void BattleScreen::Terminate()
{
    werase(m_ArenaPanelWindow);
    wrefresh(m_ArenaPanelWindow);
    delwin(m_ArenaPanelWindow);

    werase(m_BottomPanelWindow);
    wrefresh(m_BottomPanelWindow);
    delwin(m_BottomPanelWindow);

    werase(m_StatPanelWindow);
    wrefresh(m_StatPanelWindow);
    delwin(m_StatPanelWindow);

    AnimateBattleEnd();
}

int BattleScreen::SelectPlayerAction(const std::map<int, std::string>& actions)
{
    constexpr int numColumns = 3;
    const int columnWidth
        = std::max_element(actions.begin(),
                           actions.end(),
                           [&](const auto& a, const auto& b) { return a.second.size() < b.second.size(); })
              ->second.size()
          + 4;
    const int width = (columnWidth + 1) * numColumns - 1;

    return Screen::SelectViaMenu(
        actions, (ArenaPanelWidth - width) / 2 - 1, TopPanelHeight + 2, width + 2, 5, false, 0, 0, "", true, false);
}

int BattleScreen::SelectWithHoverAction(const std::map<int, std::string>& options,
                                        std::function<void(std::map<int, std::string>::iterator)> hoverAction)
{
    constexpr int width = 20;
    return Screen::SelectViaMenu(
        options, 2, TopPanelHeight + 2, width, BottomPanelHeight - 3, true, 0, 0, "", false, true, hoverAction);
}

void BattleScreen::PostMessage(const std::string& message)
{
    mvwhline(m_BottomPanelWindow, 1, 1, ' ', ArenaPanelWidth - 2);
    mvwprintw(m_BottomPanelWindow, 1, 2, message.c_str());
    wrefresh(m_BottomPanelWindow);
}

void BattleScreen::ProjectSkillUse(const Battle::AttackSkill& attackSkill)
{
    constexpr size_t arrowXPos = ArenaNameplateWidth / 2 + 6;
    int hitChancePercent = attackSkill.CalculateHitChance(m_Battle.GetPlayerProfile(), m_Battle.GetEnemyProfile());
    wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::BlackOnDefault) | A_BOLD);
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height, arrowXPos, ACS_UARROW);
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos, '|');
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 2, arrowXPos, '|');
    mvwprintw(m_ArenaPanelWindow, Components::Nameplate::Height, arrowXPos - 5, "Hit:");
    mvwprintw(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos - 5, "%3d%%", hitChancePercent);
    wattroff(m_ArenaPanelWindow, A_COLOR | A_BOLD);
    wrefresh(m_ArenaPanelWindow);
}

void BattleScreen::ClearProjectionArea()
{
    for (int i = 0; i < 3; i++)
    {
        mvwhline(m_ArenaPanelWindow, Components::Nameplate::Height + i, 0, ' ', ArenaNameplateWidth);
    }
    wrefresh(m_ArenaPanelWindow);
}

void BattleScreen::ClearThumbnailArea()
{
    for (int i = 0; i < BottomPanelHeight - 2; i++)
    {
        mvwhline(m_BottomPanelWindow, i + 1, 22, ' ', ArenaPanelWidth - 22);
    }
    mvwhline(m_BottomPanelWindow, 0, 23, ACS_HLINE, ArenaPanelWidth - 23);
    mvwvline(m_BottomPanelWindow, 1, 23, ' ', BottomPanelHeight - 1);
    mvwhline(m_BottomPanelWindow, BottomPanelHeight - 1, 23, ACS_HLINE, ArenaPanelWidth - 23);
    wrefresh(m_BottomPanelWindow);
}

void BattleScreen::AnimatePlayerAttack(const Battle::Skill::ApplySkillResult& displayData)
{
    // Constants
    constexpr size_t arrowXPos      = ArenaNameplateWidth / 2 + 6;
    constexpr int animationPeriodMs = 100;
    constexpr int missDelayMs       = 1200;

    // Begin drawing the arrow
    wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::YellowOnDefault) | A_BOLD);

    std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 2, arrowXPos, '|');
    wrefresh(m_ArenaPanelWindow);
    std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));

    if (displayData.IsHit)
    {
        // Finish drawing the arrow
        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos, '|');
        wrefresh(m_ArenaPanelWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));

        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height, arrowXPos, ACS_UARROW);
        wrefresh(m_ArenaPanelWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));

        // "Hit!"
        wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::RedOnDefault));
        mvwprintw(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos - 5, "Hit!");

        // Damage number
        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos + 2, '*');
        wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::YellowOnDefault));
        wprintw(m_ArenaPanelWindow, " %d ", displayData.Value);
        waddch(m_ArenaPanelWindow, '*' | COLOR_PAIR(ColorPairs::RedOnDefault));

        // "Crit!"
        if (displayData.IsCrit)
        {
            mvwaddstr(m_ArenaPanelWindow, Components::Nameplate::Height, arrowXPos + 2, "Critical!");
        }
        wrefresh(m_ArenaPanelWindow);

        // Log
        LogDamage(displayData, m_Battle.GetPlayer().GetName(), m_Battle.GetEnemy().GetName());
        
        // Nameplate animations
        m_EnemyNameplate.FlashBorder(ColorPairs::RedOnDefault, 2, animationPeriodMs);
        m_EnemyNameplate.HealthBar.RollBy(-displayData.Value);
    }
    else
    {
        // Draw X and "Miss!" text
        wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::RedOnDefault));
        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos, 'X');
        wrefresh(m_ArenaPanelWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
        mvwprintw(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos - 6, "Miss!");
        wrefresh(m_ArenaPanelWindow);

        // Log
        LogDamage(displayData, m_Battle.GetPlayer().GetName(), m_Battle.GetEnemy().GetName());
    }

    // Wait a bit, delay is shorter if hit due to animations
    std::this_thread::sleep_for(std::chrono::milliseconds(missDelayMs / (displayData.IsHit ? 2 : 1)));
    wattroff(m_ArenaPanelWindow, A_COLOR | A_BOLD);
    wrefresh(m_ArenaPanelWindow);

    ClearProjectionArea();
}

void BattleScreen::AnimateEnemyAttack(const Battle::Skill::ApplySkillResult& displayData, const std::string& skillName)
{
    // Constants
    constexpr size_t arrowXPos      = ArenaNameplateWidth / 2 - 7;
    constexpr int animationPeriodMs = 100;
    constexpr int missDelayMs       = 1200;
    constexpr int preAttackDelayMs  = 800;

    // Skill name
    std::string nameYell = skillName + "!";
    wattron(m_ArenaPanelWindow, A_BOLD);
    mvwaddstr(m_ArenaPanelWindow, Components::Nameplate::Height, arrowXPos + 2, nameYell.c_str());
    wrefresh(m_ArenaPanelWindow);
    std::this_thread::sleep_for(std::chrono::milliseconds(preAttackDelayMs));

    // Begin drawing the arrow
    wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::YellowOnDefault));

    std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height, arrowXPos, '|');
    wrefresh(m_ArenaPanelWindow);
    std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));

    if (displayData.IsHit)
    {
        // Finish drawing the arrow
        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos, '|');
        wrefresh(m_ArenaPanelWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));

        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 2, arrowXPos, ACS_DARROW);
        wrefresh(m_ArenaPanelWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));

        // Damage number
        wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::RedOnDefault));
        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos + 2, '*');
        wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::YellowOnDefault));
        wprintw(m_ArenaPanelWindow, " %d ", displayData.Value);
        waddch(m_ArenaPanelWindow, '*' | COLOR_PAIR(ColorPairs::RedOnDefault));

        // "Crit!"
        if (displayData.IsCrit)
        {
            mvwaddstr(m_ArenaPanelWindow, Components::Nameplate::Height + 2, arrowXPos + 2, "Critical!");
        }
        wrefresh(m_ArenaPanelWindow);

        // Log
        LogDamage(displayData, m_Battle.GetEnemy().GetName(), m_Battle.GetPlayer().GetName());

        // Nameplate animations
        m_PlayerNameplate.FlashBorder(ColorPairs::RedOnDefault, 2, animationPeriodMs);
        m_PlayerNameplate.HealthBar.RollBy(-displayData.Value);
    }
    else
    {
        // Draw X and "Miss!" text
        wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::RedOnDefault));
        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos, 'X');
        wrefresh(m_ArenaPanelWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
        mvwprintw(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos + 2, "Miss!");
        wrefresh(m_ArenaPanelWindow);

        // Log
        LogDamage(displayData, m_Battle.GetEnemy().GetName(), m_Battle.GetPlayer().GetName());
    }

    // Wait a bit, delay is shorter if hit due to animations
    std::this_thread::sleep_for(std::chrono::milliseconds(missDelayMs / (displayData.IsHit ? 2 : 1)));
    wattroff(m_ArenaPanelWindow, A_COLOR | A_BOLD);
    wrefresh(m_ArenaPanelWindow);

    ClearProjectionArea();
}

void BattleScreen::BattleEndMessage(BattleResult result)
{
    ClearBottomPanel();

    switch (result)
    {
    case BattleResult::Victory:
        mvwprintw(m_BottomPanelWindow,
                  1,
                  2,
                  "%s has defeated %s!",
                  m_Battle.GetPlayer().GetName().c_str(),
                  m_Battle.GetEnemy().GetName().c_str());
        break;
    case BattleResult::GameOver:
        mvwprintw(m_BottomPanelWindow,
                  1,
                  2,
                  "%s has been slain by %s...",
                  m_Battle.GetPlayer().GetName().c_str(),
                  m_Battle.GetEnemy().GetName().c_str());
        break;
    case BattleResult::Escape:
        mvwprintw(m_BottomPanelWindow, 1, 2, "%s runs away from the battle!", m_Battle.GetPlayer().GetName().c_str());
        break;
    default:
        break;
    }
    wrefresh(m_BottomPanelWindow);

    wgetch(m_BottomPanelWindow);
}

void BattleScreen::DisplayPlayerStats()
{
    auto& stats = m_Battle.GetPlayerProfile().Stats;

    for (int i = 1; i <= 6; i++)
    {
        mvwhline(m_StatPanelWindow, i, 1, ' ', LogPanelWidth - 2);
    }

    mvwaddstr(m_StatPanelWindow, 2, 5, "Strength: ");
    wattron(m_StatPanelWindow, A_BOLD | COLOR_PAIR(ColorPairs::RedOnDefault));
    wprintw(m_StatPanelWindow, "%9d", stats.Strength);
    wattroff(m_StatPanelWindow, A_BOLD | A_COLOR);

    mvwaddstr(m_StatPanelWindow, 3, 5, "Dexterity: ");
    wattron(m_StatPanelWindow, A_BOLD | COLOR_PAIR(ColorPairs::GreenOnDefault));
    wprintw(m_StatPanelWindow, "%8d", stats.Dexterity);
    wattroff(m_StatPanelWindow, A_BOLD | A_COLOR);

    mvwaddstr(m_StatPanelWindow, 4, 5, "Sorcery: ");
    wattron(m_StatPanelWindow, A_BOLD | COLOR_PAIR(ColorPairs::MagentaOnDefault));
    wprintw(m_StatPanelWindow, "%10d", stats.Sorcery);
    wattroff(m_StatPanelWindow, A_BOLD | A_COLOR);

    mvwaddstr(m_StatPanelWindow, 5, 5, "Wisdom: ");
    wattron(m_StatPanelWindow, A_BOLD | COLOR_PAIR(ColorPairs::BlueOnDefault));
    wprintw(m_StatPanelWindow, "%11d", stats.Wisdom);
    wattroff(m_StatPanelWindow, A_BOLD | A_COLOR);

    wrefresh(m_StatPanelWindow);
}

void BattleScreen::DrawSkillHoverThumbnailBase(const Battle::Skill& skill)
{
    // Borders
    mvwvline(m_BottomPanelWindow, 1, SkillHoverThumbnailXPos, ACS_VLINE, BottomPanelHeight - 1);
    mvwaddch(m_BottomPanelWindow, 0, SkillHoverThumbnailXPos, ACS_TTEE);
    mvwaddch(m_BottomPanelWindow, BottomPanelHeight - 1, SkillHoverThumbnailXPos, ACS_BTEE);

    // Thumbnail title
    std::string title = " " + skill.GetName() + " ";
    wattron(m_BottomPanelWindow, A_REVERSE);
    mvwaddstr(m_BottomPanelWindow, 0, (SkillHoverThumbnailXPos + ArenaPanelWidth - title.size()) / 2, title.c_str());
    wattroff(m_BottomPanelWindow, A_REVERSE);

    // Cost
    int manaCost = 0;
    if (manaCost > 0)
    {
        mvwaddstr(m_BottomPanelWindow, 5, SkillHoverThumbnailXPos + 2, "Cost: ");
        wattron(m_BottomPanelWindow, COLOR_PAIR(ColorPairs::BlueOnDefault) | A_BOLD);
        wprintw(m_BottomPanelWindow, "%d", manaCost);
        wattroff(m_BottomPanelWindow, A_COLOR | A_BOLD);
    }
    // Flavor text
    mvwaddstr(m_BottomPanelWindow, 6, SkillHoverThumbnailXPos + 2, skill.GetFlavorText().c_str());

    wrefresh(m_BottomPanelWindow);
}

void BattleScreen::PrintSkillHoverThumbnailInfo(const Battle::AttackSkill& attackSkill)
{
    DrawSkillHoverThumbnailBase(attackSkill);

    // Contents
    auto damageRange
        = attackSkill.CalculateEffectiveDamageRange(m_Battle.GetPlayerProfile(), m_Battle.GetEnemyProfile());

    // Weapon
    std::string weaponName = "Unarmed"; // TODO: add once items are added
    mvwaddstr(m_BottomPanelWindow, 1, SkillHoverThumbnailXPos + 2, weaponName.c_str());

    // Attack damage
    wattron(m_BottomPanelWindow, A_BOLD);
    mvwprintw(m_BottomPanelWindow, 2, SkillHoverThumbnailXPos + 2, "%d", damageRange.first);
    wattroff(m_BottomPanelWindow, A_BOLD);
    waddstr(m_BottomPanelWindow, " - ");
    wattron(m_BottomPanelWindow, A_BOLD);
    wprintw(m_BottomPanelWindow, "%d ", damageRange.second);
    waddstr(m_BottomPanelWindow, "Physical"); // TODO: add after damage types are added
    wattroff(m_BottomPanelWindow, A_BOLD);

    // Hit & crit
    int hitChance  = attackSkill.CalculateHitChance(m_Battle.GetPlayerProfile(), m_Battle.GetEnemyProfile());
    int critChance = attackSkill.CalculateCritChance(m_Battle.GetPlayerProfile(), m_Battle.GetEnemyProfile());
    mvwaddstr(m_BottomPanelWindow, 4, SkillHoverThumbnailXPos + 2, "Hit:");
    mvwaddstr(m_BottomPanelWindow, 4, SkillHoverThumbnailXPos + 16, "Crit:");
    short hitColor  = ColorPairs::GreenOnDefault;
    short critColor = ColorPairs::GreenOnDefault;

    if (hitChance <= 80)
        hitColor = ColorPairs::YellowOnDefault;
    if (hitChance <= 40)
        hitColor = ColorPairs::RedOnDefault;
    wattron(m_BottomPanelWindow, COLOR_PAIR(hitColor) | A_BOLD);
    mvwprintw(m_BottomPanelWindow, 4, SkillHoverThumbnailXPos + 7, "%3d%%", hitChance);

    if (critChance <= 25)
        critColor = ColorPairs::YellowOnDefault;
    if (critChance <= 10)
        critColor = ColorPairs::RedOnDefault;
    wattron(m_BottomPanelWindow, COLOR_PAIR(critColor));
    mvwprintw(m_BottomPanelWindow, 4, SkillHoverThumbnailXPos + 22, "%3d%%", critChance);
    wattroff(m_BottomPanelWindow, A_COLOR | A_BOLD);

    wrefresh(m_BottomPanelWindow);
}

void BattleScreen::AppendToLog(const std::string& message)
{
    m_LogWindow.Append(message);
    m_LogWindow.RefreshContent();
}

void BattleScreen::DrawScreenLayout()
{
    DrawArenaPanel();
    DrawLogPanel();
    DrawBottomPanel();
    DrawStatPanel();
}

void BattleScreen::DrawArenaPanel()
{
    werase(m_ArenaPanelWindow);
    wrefresh(m_ArenaPanelWindow);

    m_PlayerNameplate.Draw();
    m_EnemyNameplate.Draw();
}

void BattleScreen::DrawLogPanel()
{
    WINDOW* logWindow = m_LogWindow.Draw();
    // Connect borders with the stat panel
    mvwhline(logWindow, TopPanelHeight - 1, 1, ' ', LogPanelWidth - 2);
    mvwaddch(logWindow, TopPanelHeight - 1, 0, ACS_VLINE);
    mvwaddch(logWindow, TopPanelHeight - 1, LogPanelWidth - 1, ACS_VLINE);
    wrefresh(logWindow);
}

void BattleScreen::DrawBottomPanel()
{
    werase(m_BottomPanelWindow);
    wborder(m_BottomPanelWindow, 0, ' ', 0, 0, 0, ACS_HLINE, 0, ACS_HLINE);
    wrefresh(m_BottomPanelWindow);
}

void BattleScreen::DrawStatPanel()
{
    wborder(m_StatPanelWindow, 0, 0, 0, 0, ACS_PLUS, ACS_RTEE, ACS_BTEE, 0);
    wattron(m_StatPanelWindow, A_REVERSE);
    Screen::PrintCenter(m_StatPanelWindow, " About " + m_Battle.GetPlayer().GetName() + " ", 0);
    wattroff(m_StatPanelWindow, A_REVERSE);
    DisplayPlayerStats();
    wrefresh(m_StatPanelWindow);
}

void BattleScreen::ClearBottomPanel()
{
    for (int i = 1; i < BottomPanelHeight - 1; i++)
    {
        mvwhline(m_BottomPanelWindow, i, 1, ' ', ArenaPanelWidth - 2);
    }
    wrefresh(m_BottomPanelWindow);
}

void BattleScreen::AnimateBattleStart()
{
    constexpr int animationPeriodMs = 120;

    WINDOW* tempSideWindow = newwin(Screen::ScreenHeight, LogPanelWidth, 0, ArenaPanelWidth);
    box(tempSideWindow, 0, 0);
    wrefresh(tempSideWindow);
    delwin(tempSideWindow);

    WINDOW* tempBottomWindow = newwin(BottomPanelHeight - 3, Screen::ScreenWidth, TopPanelHeight + 3, 0);

    for (int i = 3; i >= 0; i--)
    {
        werase(tempBottomWindow);
        mvwin(tempBottomWindow, TopPanelHeight + i, 0);
        wresize(tempBottomWindow, BottomPanelHeight - i, Screen::ScreenWidth);
        box(tempBottomWindow, 0, 0);
        mvwaddch(tempBottomWindow, 0, ArenaPanelWidth, ACS_PLUS);
        mvwaddch(tempBottomWindow, 0, Screen::ScreenWidth - 1, ACS_RTEE);
        mvwvline(tempBottomWindow, 1, ArenaPanelWidth, ACS_VLINE, BottomPanelHeight - i - 2);
        mvwaddch(tempBottomWindow, BottomPanelHeight - i - 1, ArenaPanelWidth, ACS_BTEE);

        wrefresh(tempBottomWindow);
        if (i > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
    }
    delwin(tempBottomWindow);
}

void BattleScreen::AnimateBattleEnd()
{
    constexpr int animationPeriodMs = 120;

    WINDOW* tempSideWindow = newwin(Screen::ScreenHeight, LogPanelWidth, 0, ArenaPanelWidth);
    box(tempSideWindow, 0, 0);
    wrefresh(tempSideWindow);

    WINDOW* tempBottomWindow = newwin(BottomPanelHeight - 3, Screen::ScreenWidth, TopPanelHeight + 3, 0);

    for (int i = 0; i < 4; i++)
    {
        mvwhline(tempBottomWindow, 0, 0, ' ', Screen::ScreenWidth);
        wrefresh(tempBottomWindow);
        wresize(tempBottomWindow, BottomPanelHeight - i, Screen::ScreenWidth);
        mvwin(tempBottomWindow, TopPanelHeight + i, 0);
        box(tempSideWindow, 0, 0);
        wrefresh(tempSideWindow);
        box(tempBottomWindow, 0, 0);
        mvwaddch(tempBottomWindow, 0, ArenaPanelWidth, ACS_PLUS);
        mvwaddch(tempBottomWindow, 0, Screen::ScreenWidth - 1, ACS_RTEE);
        mvwvline(tempBottomWindow, 1, ArenaPanelWidth, ACS_VLINE, BottomPanelHeight - i - 2);
        mvwaddch(tempBottomWindow, BottomPanelHeight - i - 1, ArenaPanelWidth, ACS_BTEE);

        wrefresh(tempBottomWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
    }
    werase(tempBottomWindow);
    wrefresh(tempBottomWindow);
    werase(tempSideWindow);
    wrefresh(tempSideWindow);
    delwin(tempSideWindow);
    delwin(tempBottomWindow);
}

void BattleScreen::LogDamage(const Battle::Skill::ApplySkillResult& result,
                             const std::string& attacker,
                             const std::string& target)
{
    if (result.IsHit)
    {
        std::ostringstream ossLog;
        ossLog << attacker << (result.IsCrit ? " critically hit " : " hit ") << target << " for " << result.Value
               << " damage!";
        AppendToLog(ossLog.str());
    }
    else
    {
        std::ostringstream ossLog;
        ossLog << attacker << " missed " << target << " with their attack!";
        AppendToLog(ossLog.str());
    }
}

} /* namespace UI */