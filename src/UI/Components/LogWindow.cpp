#include "LogWindow.h"
#include <cctype>

namespace UI::Components
{

LogWindow::LogWindow(int xPos, int yPos, int width, int height)
    : m_Width(width),
      m_Height(height),
      m_Window(newwin(height, width, yPos, xPos))
{
}

LogWindow::~LogWindow()
{
    werase(m_Window);
    wrefresh(m_Window);
    delwin(m_Window);
}

void LogWindow::Append(const std::string& message)
{
    auto lines = Split(message);
    for (auto& line : lines)
    {
        m_Log.push_back(std::move(line));
        if (static_cast<int>(m_Log.size()) > m_Height - 2)
            m_Log.pop_front();
    }
}

WINDOW* LogWindow::Draw()
{
    werase(m_Window);
    box(m_Window, 0, 0);
    RefreshContent();

    return m_Window;
}

void LogWindow::RefreshContent()
{
    // Clear the text area
    for (int i = 1 + SidePadding; i < m_Width - 2 - SidePadding; i++)
    {
        mvwvline(m_Window, 1, i, ' ', m_Height - 2);
    }

    int linum = 1;
    for (const auto& line : m_Log)
    {
        mvwaddstr(m_Window, linum, SidePadding + 1, line.c_str());
        linum++;
    }

    wrefresh(m_Window);
}

std::vector<std::string> LogWindow::Split(std::string message)
{
    const int maxLength = m_Width - 2 - SidePadding * 2;
    std::vector<std::string> output;

    bool first = true;
    while (static_cast<int>(message.size()) > maxLength)
    {
        int pos = maxLength - 3;
        while (!isspace(message.at(pos)))
        {
            // This will blow up if the message contains a single word longer than the maximum length
            // but that is a risk I'm willing to undertake
            pos--;
        }

        // Left-pad lines in a single message except the first for readability
        if (first)
        {
            output.push_back("> " + message.substr(0, pos));
        }
        else
        {
            output.push_back(" " + message.substr(0, pos));
        }

        message.erase(0, pos);
        first = false;
    }

    output.push_back(first ? "> " + message : " " + message);
    return output;
}

} // namespace UI::Components