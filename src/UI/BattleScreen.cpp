#include "BattleScreen.h"
#include "Components/FillBar.h"
#include <algorithm>
#include <chrono>
#include <ncurses.h>
#include <thread>

namespace UI
{

BattleScreen::BattleScreen(Battle::Battle& battle, Screen& screen, InputHandler& inputHandler)
    : Subscreen(screen, inputHandler),
      m_Battle(battle),
      m_ArenaPanelWindow(nullptr),
      m_LogPanelWindow(nullptr),
      m_BottomPanelWindow(nullptr),
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
        m_LogPanelWindow = newwin(TopPanelHeight + 1, LogPanelWidth, 0, ArenaPanelWidth);
    if (m_BottomPanelWindow == nullptr)
        m_BottomPanelWindow = newwin(BottomPanelHeight, Screen::ScreenWidth, TopPanelHeight, 0);

    DrawScreenLayout();
}

void BattleScreen::Terminate()
{
    wclear(m_ArenaPanelWindow);
    wrefresh(m_ArenaPanelWindow);
    delwin(m_ArenaPanelWindow);
    wclear(m_LogPanelWindow);
    wrefresh(m_LogPanelWindow);
    delwin(m_LogPanelWindow);
    wclear(m_BottomPanelWindow);
    wrefresh(m_BottomPanelWindow);
    delwin(m_BottomPanelWindow);
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

void BattleScreen::ProjectAttack(int damage, int hitChancePercent)
{
    constexpr size_t arrowXPos = ArenaNameplateWidth / 2 + 6;
    wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::BlackOnDefault) | A_BOLD);
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height, arrowXPos, ACS_UARROW);
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos, '|');
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 2, arrowXPos, '|');
    mvwprintw(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos + 2, "* %d *", damage);
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
    constexpr size_t arrowXPos      = ArenaNameplateWidth / 2 + 6;
    constexpr int animationPeriodMs = 80;
    constexpr int afterDelayMs      = 1200;

    wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::YellowOnDefault) | A_BOLD);

    std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 2, arrowXPos, '|');
    wrefresh(m_ArenaPanelWindow);
    std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
    mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos, '|');
    wrefresh(m_ArenaPanelWindow);
    std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));

    if (hit)
    {
        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height, arrowXPos, '^');
        wrefresh(m_ArenaPanelWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
        wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::RedOnDefault));
        mvwprintw(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos - 5, "Hit!");

        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos + 2, '*');
        wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::YellowOnDefault));
        wprintw(m_ArenaPanelWindow, " %d ", damage);
        waddch(m_ArenaPanelWindow, '*' | COLOR_PAIR(ColorPairs::RedOnDefault));
        wrefresh(m_ArenaPanelWindow);

        m_EnemyNameplate.FlashBorder(ColorPairs::RedOnDefault, 2, animationPeriodMs - 20);
    }
    else
    {
        wattron(m_ArenaPanelWindow, COLOR_PAIR(ColorPairs::RedOnDefault));
        mvwaddch(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos, 'X');
        wrefresh(m_ArenaPanelWindow);
        std::this_thread::sleep_for(std::chrono::milliseconds(animationPeriodMs));
        mvwprintw(m_ArenaPanelWindow, Components::Nameplate::Height + 1, arrowXPos - 6, "Miss!");
    }
    wattroff(m_ArenaPanelWindow, A_COLOR | A_BOLD);
    wrefresh(m_ArenaPanelWindow);

    std::this_thread::sleep_for(std::chrono::milliseconds(afterDelayMs));

    ClearProjectionArea();
}

void BattleScreen::DrawScreenLayout()
{
    DrawArenaPanel();
    DrawBottomPanel();
    DrawLogPanel();
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
    wborder(m_LogPanelWindow, 0, 0, 0, 0, 0, 0, ACS_BTEE, ACS_RTEE);
    wrefresh(m_LogPanelWindow);
}

void BattleScreen::DrawBottomPanel()
{
    const auto& player = m_Battle.GetPlayer();

    werase(m_BottomPanelWindow);
    box(m_BottomPanelWindow, 0, 0);
    mvwprintw(m_BottomPanelWindow, 1, 3, "What will %s do?", player.GetName().c_str());
    wrefresh(m_BottomPanelWindow);
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