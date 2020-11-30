#pragma once

#include <ncurses.h>
#include <string>

namespace UI::Components
{

/**
 * @brief Bar visualization of a value with an upper limit
 */
class FillBar
{
public:
    /**
     * @brief Constructor
     *
     * @param window window
     * @param size length of the bar
     * @param xPos X position of the leftmost edge
     * @param yPos Y position of the leftmost edge
     * @param value value to visualize
     * @param maxValue maximum value to visualize
     * @param fillColorPair color pair for the bar fill (default: 0 = inverted default colors)
     * @param showText whether or not to show the value and maximum in text (default: true = text enabled)
     * @param textInPercent whether or not to display only the percentage if text is enabled (default: false)
     */
    FillBar(WINDOW* window,
            int size,
            int xPos,
            int yPos,
            int value,
            int maxValue,
            short fillColorPair = 0,
            bool showText       = true,
            bool textInPercent  = false);

    /**
     * @brief Destructor
     */
    ~FillBar();

    /**
     * @brief Draw the bar
     */
    void Draw();

private:
    WINDOW* m_Window;
    int m_Size, m_Value, m_MaxValue;
    short m_FillColorPair;
    bool m_ShowText, m_TextInPercent;

    /**
     * @brief Calculate how much of the bar is filled
     *
     * @return int filled length
     */
    int FilledLength() const;

    /**
     * @brief Get the decimal ratio of the value against the maximum
     * 
     * @return double ratio
     */
    double Ratio() const;

    /**
     * @brief Get the text representation of the value according to settings
     *
     * @return std::string text representation
     */
    std::string TextRepresentation() const;
};

} /* namespace UI::Components */