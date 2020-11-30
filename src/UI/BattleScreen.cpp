#include "BattleScreen.h"
#include "Components/FillBar.h"
#include <ncurses.h>

namespace UI
{

BattleScreen::BattleScreen(Battle::Battle& battle, Screen& screen, InputHandler& inputHandler)
    : Subscreen(screen, inputHandler),
      m_Battle(battle),
      m_ArenaPanelWindow(nullptr),
      m_LogPanelWindow(nullptr),
      m_BottomPanelWindow(nullptr),
      m_PlayerNameplate(nullptr),
      m_EnemyNameplate(nullptr)
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

    if (m_PlayerNameplate == nullptr)
        m_PlayerNameplate = derwin(m_ArenaPanelWindow, NameplateHeight, NameplateWidth, NameplateHeight + 3, 0);
    if (m_EnemyNameplate == nullptr)
        m_EnemyNameplate = derwin(m_ArenaPanelWindow, NameplateHeight, NameplateWidth, 0, 0);

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
    wclear(m_PlayerNameplate);
    wrefresh(m_PlayerNameplate);
    delwin(m_PlayerNameplate);
    wclear(m_EnemyNameplate);
    wrefresh(m_EnemyNameplate);
    delwin(m_EnemyNameplate);
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
    wrefresh(m_ArenaPanelWindow);

    DrawPlayerNameplate();
    DrawEnemyNameplate();
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

void BattleScreen::DrawPlayerNameplate()
{
    const auto& player = m_Battle.GetPlayer();

    werase(m_PlayerNameplate);

    int row = 0;

    // Draw the box
    box(m_PlayerNameplate, 0, 0);
    std::string title = " " + player.GetName() + " ";
    wattron(m_PlayerNameplate, A_REVERSE);
    Screen::PrintCenter(m_PlayerNameplate, title, NameplateHeight - 1);
    wattroff(m_PlayerNameplate, A_REVERSE);

    // Top row
    row++;
    mvwprintw(m_PlayerNameplate, row, 4, "Level %d", player.GetStats().Level);
    mvwprintw(m_PlayerNameplate,
              row,
              NameplateWidth - 4 - player.GetDescription().size(),
              "%s",
              player.GetDescription().c_str());

    // Middle row
    row++;
    mvwprintw(m_PlayerNameplate, row, 4, "HP:");
    Components::FillBar healthBar(
        m_PlayerNameplate, 16, 8, row, player.GetStats().Health, player.GetStats().MaxHealth, ColorPairs::WhiteOnGreen);
    healthBar.Draw();

    // Bottom row
    row++;
    mvwprintw(m_PlayerNameplate, row, 4, "MP:");
    Components::FillBar manaBar(
        m_PlayerNameplate, 16, 8, row, player.GetStats().Mana, player.GetStats().MaxMana, ColorPairs::WhiteOnBlue);
    manaBar.Draw();

    wrefresh(m_PlayerNameplate);
}

void BattleScreen::DrawEnemyNameplate()
{
    const auto& enemy = m_Battle.GetEnemy();

    werase(m_EnemyNameplate);

    int row = 0;

    // Draw the box
    box(m_EnemyNameplate, 0, 0);
    std::string title = " " + enemy.GetName() + " ";
    wattron(m_EnemyNameplate, A_REVERSE);
    Screen::PrintCenter(m_EnemyNameplate, title, 0);
    wattroff(m_EnemyNameplate, A_REVERSE);

    // Top row
    row++;
    mvwprintw(m_EnemyNameplate, row, 4, "Level %d", enemy.GetStats().Level);
    mvwprintw(m_EnemyNameplate,
              row,
              NameplateWidth - 4 - enemy.GetDescription().size(),
              "%s",
              enemy.GetDescription().c_str());

    // Middle row
    row++;
    mvwprintw(m_EnemyNameplate, row, 4, "HP:");
    Components::FillBar healthBar(
        m_EnemyNameplate, 16, 8, row, enemy.GetStats().Health, enemy.GetStats().MaxHealth, ColorPairs::WhiteOnGreen);
    healthBar.Draw();

    // Bottom row
    row++;
    mvwprintw(m_EnemyNameplate, row, 4, "MP:");
    Components::FillBar manaBar(
        m_EnemyNameplate, 16, 8, row, enemy.GetStats().Mana, enemy.GetStats().MaxMana, ColorPairs::WhiteOnBlue);
    manaBar.Draw();

    wrefresh(m_EnemyNameplate);
}

} /* namespace UI */