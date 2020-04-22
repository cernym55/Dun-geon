#pragma once

#include <exception>
#include <string>

class InvalidPositionException : public std::exception
{
public:
    InvalidPositionException(const std::string& message);

    const char* what() const noexcept override;

private:
    std::string m_Message;
};