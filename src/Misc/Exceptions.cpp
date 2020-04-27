#include "Exceptions.h"

InvalidPositionException::InvalidPositionException(const std::string& message)
    : m_Message(message)
{
}

const char* InvalidPositionException::what() const noexcept
{
    return m_Message.c_str();
}

DisplayException::DisplayException(const std::string& message)
    : m_Message(message)
{
}

const char* DisplayException::what() const noexcept
{
    return m_Message.c_str();
}