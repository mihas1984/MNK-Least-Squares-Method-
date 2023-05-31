#ifndef READFILE_HPP
#define READFILE_HPP
#include <vector>
#include <fstream>

/// \fn readData()
/// \brief reads data points from the file opened in the \a fin to appropriate vectors
/// (look in README.txt on how data should be placed in a file)
/// first two lines of file dedicated to variable names(first) and units(second)
/// only accepts two data points per line(divided by spaces). throws derived classes of std::exception on errors found
/// \param varNames
/// \param varUnits
/// \param vecX - vector of x values of experimental data points
/// \param vecY - vector of y values of experimental data points
/// \param fin - opened std::ifstream we will read data from
/// \return nothing
void readData(std::vector<std::string>& varNames, std::vector<std::string>& varUnits, std::vector<long double>& vecX, std::vector<long double>& vecY, std::ifstream& fin);

#endif // READFILE_HPP
