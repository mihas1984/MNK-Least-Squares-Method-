#ifndef MNK_HPP
#define MNK_HPP
#include <vector>
#include <string>

///
/// \fn computate_equation(params)
/// \brief computates the linear approximation equation using a well-known formula (least squares method).
/// the equation will be "y = slope * x + intercept"
/// \param x - vector of x values of experimental data points
/// \param y - vector of y values of experimental data points
/// \param slope - slope of the resulting equation
/// \param intecept - intercept of the resulting equation
/// \param error - empty string which will be populated with message if errors occured
/// \return \c 0 if the work is correct, \c -1 if the data cannot be fitted
int computate_equation(const std::vector<long double> x, const std::vector<long double> y, long double& slope, long double& intercept, std::string& errorFlag);
///
/// \fn computate_equation_error(params)
/// \brief computates the linear approximation errors for intercept and slope using a well-known formula (least squares method).
/// \param x - vector of x values of experimental data points
/// \param y - vector of y values of experimental data points
/// \param slope - slope of the equation
/// \param intecept - intercept of the equation
/// \param slope_error - calculated standard error for slope
/// \param intercept_error - calculated standard error for intercept
/// \return \c 0 if everything goes right, \c -1 if there are no slope and intercept errors (errors = 0)
int computate_equation_error(const std::vector<long double> x, const std::vector<long double> y, const long double slope,
                             const long double intercept, long double& slope_error, long double& intercept_error);

#endif // MNK_HPP
