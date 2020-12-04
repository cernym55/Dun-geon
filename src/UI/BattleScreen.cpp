#include "BattleScreen.h"
#include "Components/FillBar.h"
#include <algorithm>
#include <chrono>
#include <ncurses.h>
#include <thread>

using BattleResult = Battle::Battle::Result;

namespace UI
{

BattleScreen::BattleScreen(Battle::Battle& battle, Screen& screen, InputHandler& inputHandler)
    : Subscreen(screen, inputHandler),
      m_Battle(battle),
      m_ArenaPanelWindow(nullptr),
      m_LogPanelWindow(nullptr),
      m_BottomPanelWindow(nullptr),
      m_StatPanelWindow(nullptr),
      m_PlayerNameplate(m_Battle.GetPlayer(),
                        (ArenaPanelWidth - ArenaNameplateWidth) / 2,
                        2 + Components::Nameplate::Height + 3,
                        ArenaNameplateWidth,
                        false),
      m_EnemyNameplate(m_Battle.GetEnemy(), (ArenaPanelWidth - ArenaNameplateWidth) / 2, 2, ArenaNameplateWidth, true)
{
    Init();
}

BattleScreen::~BattleScreen()
{
    Terminate();
}

void BattleScreen::Init()
{
    m_Screen.Clear();

    if (m_ArenaPanelWindow == nullptr)
        m_ArenaPanelWindow = newwin(
            Components::Nameplate::Height * 2 + 3, ArenaNameplateWidth, 2, (ArenaPanelWidth - ArenaNameplateWidth) / 2);
    if (m_LogPanelWindow == nullptr)
        m_LogPanelWindow = newwin(TopPanelHeight, LogPanelWidth, 0, ArenaPanelWidth);
    if (m_BottomPanelWindow == nullptr)
        m_BottomPanelWindow = newwin(BottomPanelHeight, ArenaPanelWidth, TopPanelHeight, 0);
    if (m_StatPanelWindow == nullptr)
        m_StatPanelWindow = newwin(BottomPanelHeight, LogPanelWidth, TopPanelHeight, ArenaPanelWidth);

    DrawScreenLayout();
}

void BattleScreen::Terminate()
{
    werase(m_ArenaPanelWindow);
    wrefresh(m_ArenaPanelWindow);
    delwin(m_ArenaPanelWindow);
    werase(m_LogPanelWindow);
    wrefresh(m_LogPanelWindow);
    delwin(m_LogPanelWindow);
    werase(m_BottomPanelWindow);
    wrefresh(m_BottomPanelWindow);
    delwin(m_BottomPanelWindow);
    werase(m_StatPanelWindow);
    wrefresh(m_StatPanelWindow);
    delwin(m_StatPanelWindow);
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

void BattleScreen::ProjectAttack(int hitChancePercent)
{
    constexpr size_t arrowXPos = ArenaNameplateWidth / 2 + 6;
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
    mvwhline(m_ArenaPanelWindow, Components::Nameplate::Height, 0, ' ', ArenaNameplateWidth);
    mvwhline(m_ArenaPanelWindow, Components::Nameplate::Height + 1, 0, ' ', ArenaNameplateWidth);
    mvwhline(m_ArenaPanelWindow, Components::Nameplate::Height + 2, 0, ' ', ArenaNameplateWidth);
    wrefresh(m_ArenaPanelWindow);
}

void BattleScreen::AnimatePlayerAttack(int damage, bool hit)
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

    if (hit)
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
        wprintw(m_ArenaPanelWindow, " %d ", damage);
        waddch(m_ArenaPanelWindow, '*' | COLOR_PAIR(ColorPairs::RedOnDefault));
        wrefresh(m_ArenaPanelWindow);

        // Nameplate animations
        m_EnemyNameplate.FlashBorder(ColorPairs::RedOnDefault, 2, animationPeriodMs);
        m_EnemyNameplate.HealthBar.RollBy(-damage);
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
    }

    // Wait a bit, delay is shorter if hit due to animations
    std::this_thread::sleep_for(std::chrono::milliseconds(missDelayMs / (hit ? 2 : 1)));
    wattroff(m_ArenaPanelWindow, A_COLOR | A_BOLD);
    wrefresh(m_ArenaPanelWindow);

    ClearProjectionArea();
}

void BattleScreen::AnimateEnemyAttack(int damage, bool hit)
{
    // Constants
    constexpr size_t arrowXPos      = ArenaNameplateWidth / 2 - 7;
    constexpr int animationPeriodMs = 100;
    constexpr int missDelayMs       = 1200;
    constexpr int preAttackDelayMs  = 800;

    // Skill name
    wattron(m_ArenaPanelWindow, A_BOLD);
    Screen::PrintCenter(m_ArenaPanelWindow, "Punch!", Components::Nameplate::Height);
    std::this_thread::sleep_for(std::chrono::milliseconds(preAttackDelayMs));

    // Begin drawing the arrow
    wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::YellowOnDefault));

    std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height, arrowXPos, '|');
    wrefresh(m_ArenaPanelWindow);
    std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));

    if (hit)
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
        wprintw(m_ArenaPanelWindow, " %d ", damage);
        waddch(m_ArenaPanelWindow, '*' | COLOR_PAIR(ColorPairs::RedOnDefault));
        wrefresh(m_ArenaPanelWindow);

        // Nameplate animations
        m_PlayerNameplate.FlashBorder(ColorPairs::RedOnDefault, 2, animationPeriodMs);
        m_PlayerNameplate.HealthBar.RollBy(-damage);
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
    }

    // Wait a bit, delay is shorter if hit due to animations
    std::this_thread::sleep_for(std::chrono::milliseconds(missDelayMs / (hit ? 2 : 1)));
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
    auto& stats = m_Battle.GetPlayerStats();

    for (int i = 1; i <= 6; i++)
    {
        mvwhline(m_StatPanelWindow, i, 1, ' ', LogPanelWidth - 2);
    }

    mvwaddstr(m_StatPanelWindow, 2, 5, "Strength: ");
    wattron(m_StatPanelWindow, A_BOLD | COLOR_PAIR(ColorPairs::RedOnDefault));
    wprintw(m_StatPanelWindow, "%9d", stats.Strength);
    wattroff(m_StatPanelWindow, A_BOLD | A_COLOR);

    mvwaddstr(m_StatPanelWindow, 3, 5, "Toughness: ");
    wattron(m_StatPanelWindow, A_BOLD | COLOR_PAIR(ColorPairs::YellowOnDefault));
    wprintw(m_StatPanelWindow, "%8d", stats.Toughness);
    wattroff(m_StatPanelWindow, A_BOLD | A_COLOR);

    mvwaddstr(m_StatPanelWindow, 4, 5, "Dexterity: ");
    wattron(m_StatPanelWindow, A_BOLD | COLOR_PAIR(ColorPairs::GreenOnDefault));
    wprintw(m_StatPanelWindow, "%8d", stats.Dexterity);
    wattroff(m_StatPanelWindow, A_BOLD | A_COLOR);

    mvwaddstr(m_StatPanelWindow, 5, 5, "Intelligence: ");
    wattron(m_StatPanelWindow, A_BOLD | COLOR_PAIR(ColorPairs::BlueOnDefault));
    wprintw(m_StatPanelWindow, "%5d", stats.Intelligence);
    wattroff(m_StatPanelWindow, A_BOLD | A_COLOR);

    wrefresh(m_StatPanelWindow);
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
    werase(m_LogPanelWindow);
    box(m_LogPanelWindow, 0, 0);
    wborder(m_LogPanelWindow, 0, 0, 0, ' ', 0, 0, ACS_VLINE, ACS_VLINE);
    wrefresh(m_LogPanelWindow);
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

} /* namespace UI */