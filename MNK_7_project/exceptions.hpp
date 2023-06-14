#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <string>
#include <stdexcept>

class InvalidData : public std::exception
{
private:
    std::string m_error;
public:
    InvalidData(std::string error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.c_str();
    }
};

class InvalidNames : public std::exception
{
private:
    std::string m_error;
public:
    InvalidNames(std::string error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.c_str();
    }
};

class InvalidDeviations : public std::exception
{
private:
    std::string m_error;
public:
    InvalidDeviations(std::string error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.c_str();
    }
};

class InvalidComputation : public std::exception
{
private:
    std::string m_error;
public:
    InvalidComputation(std::string error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.c_str();
    }
};

class ComputationWarning : public std::exception
{
private:
    std::string m_error;
public:
    ComputationWarning(std::string error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.c_str();
    }
};

#endif // EXCEPTIONS_HPP
