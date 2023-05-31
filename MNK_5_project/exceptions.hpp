#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <string>
#include <stdexcept>

class InvalidFileData : public std::exception//thrown in readfile.hpp functions
{
private:
    std::string m_error;
public:
    InvalidFileData(std::string error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.c_str();
    }
};

class InvalidComputation : public std::exception//thrown in mnk.hpp functions
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

class ComputationWarning : public std::exception//thrown in mnk.hpp functions
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

class FileError : public std::exception//thrown in mnk.hpp functions
{
private:
    std::string m_error;
public:
    FileError(std::string error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.c_str();
    }
};

#endif // EXCEPTIONS_HPP
