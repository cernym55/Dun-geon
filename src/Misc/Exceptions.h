#pragma once

#include <exception>
#include <string>

/**
 * @brief Thrown when an error occurs which involves invalid coordinates or map positions
 */
class InvalidPositionException : public std::exception
{
public:
    InvalidPositionException(const std::string& message);

    const char* what() const noexcept override;

private:
    std::string m_Message;
};

/**
 * @brief Thrown when an error occurs while displaying a UI element
 */
class DisplayException : public std::exception
{
public:
    DisplayException(const std::string& message);

    const char* what() const noexcept override;

private:
    std::string m_Message;
};