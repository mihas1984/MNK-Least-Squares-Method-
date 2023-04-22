#ifndef MNK_HPP
#define MNK_HPP
#include <vector>
#include <string>

/// \fn computate(const std::vector<double> x, const std::vector<double> y, double& a, double& b, std::string& error)
/// \brief computates the linear approximation equation using a well-known formula (least squares method).
/// the equation will be "y = slope * x + intercept"
/// \param x - vector of x values of experimental data points
/// \param y - vector of y values of experimental data points
/// \param a - slope
/// \param b - intercept
/// \param error - empty string which will be populated with message if errors occured
/// \return \c 0 if the work is correct, \c -1 if the data cannot be fitted
///
int computate(const std::vector<double> x, const std::vector<double> y, double& a, double& b, std::string& error);

#endif // MNK_HPP
