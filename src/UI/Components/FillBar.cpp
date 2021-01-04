#include "FillBar.h"
#include "Misc/Utils.h"
#include <cmath>

namespace UI::Components
{

FillBar::FillBar(WINDOW* window,
                 int size,
                 int xPos,
                 int yPos,
                 int value,
                 int maxValue,
                 short fillColorPair,
                 bool showText,
                 bool textInPercent)
    : m_Window(derwin(window, 1, size, yPos, xPos)),
      m_Size(size),
      m_Value(value),
      m_MaxValue(maxValue),
      m_FillColorPair(fillColorPair),
      m_ShowText(showText),
      m_TextInPercent(textInPercent)
{
}

FillBar::~FillBar()
{
    delwin(m_Window);
}

void FillBar::Draw(int highlightFillBeyondValue)
{
    werase(m_Window);
    mvwaddch(m_Window, 0, 0, '[' | A_BOLD);
    mvwaddch(m_Window, 0, m_Size - 1, ']' | A_BOLD);

    if (m_MaxValue == 0)
    {
        mvwaddstr(m_Window, 0, (m_Size - 3) / 2, "N/A");
        wrefresh(m_Window);
        return;
    }

    if (m_FillColorPair != 0)
        wattron(m_Window, COLOR_PAIR(m_FillColorPair) | A_BOLD);
    else
        wattron(m_Window, A_REVERSE);

    std::string text         = TextRepresentation();
    int filledLength         = FilledLength();
    int nonHighlightedLength = filledLength;

    if (highlightFillBeyondValue >= 0)
    {
        nonHighlightedLength = ceil((m_Size - 2) * static_cast<double>(highlightFillBeyondValue) / m_MaxValue);
    }

    int textBeginPos = (m_Size - text.size()) / 2 - 1;

    for (int i = 0; i < m_Size - 2; i++)
    {
        if (i == nonHighlightedLength)
        {
            wattroff(m_Window, A_COLOR);
            wattron(m_Window, A_REVERSE);
        }

        if (i == filledLength)
            wattroff(m_Window, A_COLOR | A_REVERSE);

        if (i < textBeginPos || i >= textBeginPos + static_cast<int>(text.size()))
        {
            mvwaddch(m_Window, 0, i + 1, ' ');
        }
        else
        {
            mvwaddch(m_Window, 0, i + 1, text[i - textBeginPos]);
        }
    }

    wattroff(m_Window, A_COLOR | A_REVERSE | A_BOLD);
    wrefresh(m_Window);
}

void FillBar::MoveBy(int value)
{
    m_Value += value;
    if (m_Value < 0)
        m_Value = 0;
    else if (m_Value > m_MaxValue)
        m_Value = m_MaxValue;
    Draw();
}

void FillBar::RollBy(int value)
{
    constexpr int delayMs = 60;
    int targetValue       = m_Value + value;
    if (targetValue < 0)
        targetValue = 0;
    else if (targetValue > m_MaxValue)
        targetValue = m_MaxValue;
    int sgn = value >= 0 ? 1 : -1;

    while (m_Value != targetValue)
    {
        int diff = abs(m_Value - targetValue);
        m_Value += diff != 1 ? sgn * ceil(diff / 10.0) : sgn;

        if (sgn < 0 && m_Value < targetValue)
            m_Value = targetValue;
        if (sgn > 0 && m_Value > targetValue)
            m_Value = targetValue;
        Draw(targetValue);
        Sleep(delayMs);
    }
}

int FillBar::FilledLength() const
{
    return ceil((m_Size - 2) * Ratio());
}

double FillBar::Ratio() const
{
    return static_cast<double>(m_Value) / m_MaxValue;
}

std::string FillBar::TextRepresentation() const
{
    if (!m_TextInPercent)
        return std::to_string(m_Value) + "/" + std::to_string(m_MaxValue);

    int intPercentage = static_cast<int>(ceil(Ratio() * 100));
    return std::to_string(intPercentage) + "%";
}

} /* namespace UI::Components */