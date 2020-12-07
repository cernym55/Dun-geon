#pragma once

#include <ncurses.h>
#include <deque>
#include <string>
#include <vector>

namespace UI::Components
{

/**
 * @brief Window with support for appending text messages in logging
 */
class LogWindow
{
public:
    /**
     * @brief Constructor
     *
     * @param xPos X position
     * @param yPos Y position
     * @param width width
     * @param height height
     */
    LogWindow(int xPos, int yPos, int width, int height);

    /**
     * @brief Destructor
     */
    ~LogWindow();

    /**
     * @brief Append a message to the log
     * 
     * @param message message
     */
    void Append(const std::string& message);

    /**
     * @brief Draw the log window
     * 
     * @return WINDOW* window (for additional custom changes)
     */
    WINDOW* Draw();

    /**
     * @brief Redraw the contents of the log
     */
    void RefreshContent();

private:
    constexpr static const int SidePadding = 1;
    int m_Width, m_Height;
    WINDOW* m_Window;
    std::deque<std::string> m_Log;

    /**
     * @brief Split a message into a list of sub-messages that fit the window's width.
     * 
     * @param message message
     * @return std::vector<std::string> split message
     */
    std::vector<std::string> Split(std::string message);
};

} /* namespace UI::Components */