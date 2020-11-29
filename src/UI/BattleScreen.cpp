#include "BattleScreen.h"
#include <ncurses.h>

namespace UI
{

BattleScreen::BattleScreen(Battle::Battle& battle, Screen& screen, InputHandler& inputHandler)
    : Subscreen(screen, inputHandler),
      m_Battle(battle),
      m_ArenaPanelWindow(nullptr),
      m_LogPanelWindow(nullptr),
      m_BottomPanelWindow(nullptr)
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
        m_ArenaPanelWindow = newwin(NameplateHeight * 2 + 3, NameplateWidth, 2, (ArenaPanelWidth - NameplateWidth) / 2);
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

void BattleScreen::DrawScreenLayout()
{
    DrawArenaPanel();
    DrawBottomPanel();
    DrawLogPanel();
    getch(); // TODO: remove
}

void BattleScreen::DrawArenaPanel()
{
    werase(m_ArenaPanelWindow);
    box(m_ArenaPanelWindow, 0, 0);
    wrefresh(m_ArenaPanelWindow);
}

void BattleScreen::DrawLogPanel()
{
    werase(m_LogPanelWindow);
    wborder(m_LogPanelWindow, 0, 0, 0, 0, 0, 0, ACS_BTEE, ACS_RTEE);
    wrefresh(m_LogPanelWindow);
}

void BattleScreen::DrawBottomPanel()
{
    werase(m_BottomPanelWindow);
    box(m_BottomPanelWindow, 0, 0);
    wrefresh(m_BottomPanelWindow);
}

} /* namespace UI */