#pragma once

#include "Entities/Character.h"
#include "FillBar.h"
#include <ncurses.h>

namespace UI::Components
{

/**
 * @brief Nameplate UI component
 */
class Nameplate
{
public:
    /**
     * @brief Height of the nameplate
     */
    constexpr static const int Height = 5;

    /**
     * @brief Constructor
     *
     * @param character character displayed
     * @param xPos X position
     * @param yPos Y position
     * @param width width
     * @param isTitleOnTop true if title (name) should be on top, false if on bottom
     */
    Nameplate(const Entities::Character& character, int xPos, int yPos, int width, bool isTitleOnTop);

    /**
     * @brief Destructor
     */
    ~Nameplate();

    /**
     * @brief Draw the nameplate
     */
    void Draw();

    /**
     * @brief Flash the nameplate border
     *
     * @param colorPair flash color
     * @param count how many times to flash
     * @param periodMs period between flashes in milliseconds
     */
    void FlashBorder(short colorPair, int count, int periodMs);

private:
    const Entities::Character& m_Character;
    int m_Width;
    bool m_IsTitleOnTop;
    WINDOW* m_Window;

    /**
     * @brief Draw the border
     */
    void DrawBorder();

public:
    /**
     * @brief Health bar
     */
    FillBar HealthBar;

    /**
     * @brief Mana bar
     */
    FillBar ManaBar;
};

} /* namespace UI::Components */