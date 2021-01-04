#include "Subscreen.h"
#include "Screen.h"

namespace UI
{

Subscreen::Subscreen(Screen& screen, InputHandler& inputHandler) : m_Screen(screen), m_InputHandler(inputHandler)
{
}

} // namespace UI