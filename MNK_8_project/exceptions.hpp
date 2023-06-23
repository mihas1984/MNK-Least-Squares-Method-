#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <QString>
#include <stdexcept>

class InvalidData : public std::exception
{
private:
    QString m_error;
public:
    InvalidData(QString error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.toStdString().c_str();
    }
};

class InvalidNames : public std::exception
{
private:
    QString m_error;
public:
    InvalidNames(QString error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.toStdString().c_str();
    }
};

class InvalidDeviations : public std::exception
{
private:
    QString m_error;
public:
    InvalidDeviations(QString error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.toStdString().c_str();
    }
};

class InvalidComputation : public std::exception
{
private:
    QString m_error;
public:
    InvalidComputation(QString error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.toStdString().c_str();
    }
};

class ComputationWarning : public std::exception
{
private:
    QString m_error;
public:
    ComputationWarning(QString error) : m_error(error)
    {
    }
    const char* what() const noexcept
    {
        return m_error.toStdString().c_str();
    }
};

#endif // EXCEPTIONS_HPP
