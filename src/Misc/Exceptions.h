#pragma once

#include <exception>
#include <string>

/**
 * @brief Customized base exception type
 *
 */
class CustomException : public std::exception
{
public:
    CustomException(const std::string& message) : m_Message(message) {}

    const char* what() const noexcept override { return m_Message.c_str(); }

private:
    std::string m_Message;
};

/**
 * @brief Thrown when an error occurs while displaying a UI element
 */
class DisplayException : public CustomException
{
public:
    DisplayException(const std::string& message) : CustomException(message) {}
};

/**
 * @brief Thrown when attempting to perform an operation with an invalid enum value
 * 
 */
class InvalidEnumValueException : public CustomException
{
public:
    InvalidEnumValueException(const std::string& message) : CustomException(message) {}
};

/**
 * @brief Thrown when an error occurs which involves invalid coordinates or map positions
 */
class InvalidPositionException : public CustomException
{
public:
    InvalidPositionException(const std::string& message) : CustomException(message) {}
};

/**
 * @brief Thrown when an unsupported operation is performed
 */
class NotSupportedException : public CustomException
{
public:
    NotSupportedException(const std::string& message) : CustomException(message) {}
};