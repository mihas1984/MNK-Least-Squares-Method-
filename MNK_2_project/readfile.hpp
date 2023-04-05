#ifndef READFILE_HPP
#define READFILE_HPP
#include <vector>
#include <fstream>
///
/// \brief reads data points from the file opened in the \a fin to appropriate vectors
/// (look in README.txt on how data should be placed in a file)
/// \param vecX - vector of x values of experimental data points
/// \param vecY - vector of y values of experimental data points
/// \param fin - input filestream with an "opened"(it is tested if it realy is) file
/// \param error - empty string which will be populated with message if errors occured
/// \return \c 0 if the work is correct, \c -1 if the file cannot be opened or data is corrupt
///
int readData(std::vector<double>& vecX, std::vector<double>& vecY, std::ifstream& fin, std::string& error);

#endif // READFILE_HPP
