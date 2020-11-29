#pragma once

#include "InputHandler.h"

namespace UI
{

class Screen;

/**
 * @brief Screen controller for displaying a specific game event
 */
class Subscreen
{
public:
    /**
     * @brief Constructor
     *
     * @param screen screen
     * @param inputHandler input handler
     */
    Subscreen(Screen& screen, InputHandler& inputHandler);

    /**
     * @brief Destructor
     */
    virtual ~Subscreen() = default;

    /**
     * @brief Initialize the subscreen
     */
    virtual void Init() = 0;

    /**
     * @brief Clean up after the subscreen
     */
    virtual void Terminate() = 0;

protected:
    Screen& m_Screen;
    InputHandler& m_InputHandler;
};

} /* namespace UI */