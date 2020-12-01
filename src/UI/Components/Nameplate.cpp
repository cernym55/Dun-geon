#include "Nameplate.h"
#include "ColorPairs.h"
#include "FillBar.h"
#include "Screen.h"
#include <chrono>
#include <thread>

namespace UI::Components
{

Nameplate::Nameplate(const Entities::Character& character, int xPos, int yPos, int width, bool isTitleOnTop)
    : m_Character(character),
      m_Width(width),
      m_IsTitleOnTop(isTitleOnTop),
      m_Window(newwin(Height, width, yPos, xPos))
{
}

Nameplate::~Nameplate()
{
    wclear(m_Window);
    wrefresh(m_Window);
    delwin(m_Window);
}

void Nameplate::Draw()
{
    werase(m_Window);
    DrawBorder();

    int row = 0;

    // Top row
    row++;
    mvwprintw(m_Window, row, 4, "Level %d", m_Character.GetStats().Level);
    mvwprintw(
        m_Window, row, m_Width - 4 - m_Character.GetDescription().size(), "%s", m_Character.GetDescription().c_str());

    // Middle row
    row++;
    mvwprintw(m_Window, row, 4, "HP:");
    FillBar healthBar(m_Window,
                      16,
                      8,
                      row,
                      m_Character.GetStats().Health,
                      m_Character.GetStats().MaxHealth,
                      ColorPairs::WhiteOnGreen);
    healthBar.Draw();

    // Bottom row
    row++;
    mvwprintw(m_Window, row, 4, "MP:");
    FillBar manaBar(
        m_Window, 16, 8, row, m_Character.GetStats().Mana, m_Character.GetStats().MaxMana, ColorPairs::WhiteOnBlue);
    manaBar.Draw();

    wrefresh(m_Window);
}

void Nameplate::FlashBorder(short colorPair, int count, int periodMs)
{
    const std::chrono::milliseconds delay(periodMs);

    for (int i = 0; i < count; i++)
    {
        if (i != 0)
            std::this_thread::sleep_for(delay);

        wattron(m_Window, COLOR_PAIR(colorPair));
        DrawBorder();

        std::this_thread::sleep_for(delay);

        wattroff(m_Window, A_COLOR);
        DrawBorder();
    }
}

void Nameplate::DrawBorder()
{
    box(m_Window, 0, 0);
    std::string title = " " + m_Character.GetName() + " ";
    wattron(m_Window, A_REVERSE);
    Screen::PrintCenter(m_Window, title, m_IsTitleOnTop ? 0 : Height - 1);
    wattroff(m_Window, A_REVERSE);
    wrefresh(m_Window);
}

} // namespace UI::Components