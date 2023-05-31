#include "readfile.hpp"
#include "exceptions.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

void readData(std::vector<std::string>& varNames, std::vector<std::string>& varUnits, std::vector<long double>& vecX, std::vector<long double>& vecY, std::ifstream& fin){
    if (fin)
    {
        std::stringstream ss;
        ss << fin.rdbuf();
        std::string line;
        // Read the first line
        if (std::getline(ss, line)) {
            std::istringstream lineStream(line);
            std::string xName {""};
            std::string yName {""};
            std::string other { "" };
            if((lineStream >> xName >> yName) && !(lineStream >> other)){//if there's not exactly 2 words in a line and nothing else
                varNames.push_back(xName);
                varNames.push_back(yName);
            }
            else{
                fin.close();
                throw InvalidFileData("ERROR: invalid variable names in the file.\n Please check if there are unnecessary spaces or all of the names are there.");
            }
        } else {
            fin.close();
            throw InvalidFileData("The first line in a file does not contain variable names.");
        }
        // Read the second line
        if (std::getline(ss, line)) {
            std::istringstream lineStream(line);
            std::string xUnits {0};
            std::string yUnits {0};
            std::string other { "" };
            if((lineStream >> xUnits >> yUnits) && !(lineStream >> other)){//if there's not exactly 2 words in a line and nothing else
                varUnits.push_back(xUnits);
                varUnits.push_back(yUnits);
            }
            else{
                fin.close();
                throw InvalidFileData("ERROR: invalid variable units in the file.\n Please check if there are unnecessary spaces or all of the unit names are there.");
            }
        } else {
            fin.close();
            throw InvalidFileData("The second line in a file does not contain variable units.");
        }
        //splitting left data line by line
        while (std::getline(ss, line)) {
            if(!(line.find_first_not_of(' ') != std::string::npos))
            {// There are spaces only or empty
                continue;
            }
            std::replace(line.begin(), line.end(), ',', '.');//replacing commas with dots if such available
            std::istringstream lineStream(line);
            long double x {0};
            long double y {0};
            std::string other { "" };
            if((lineStream >> x >> y) && !(lineStream >> other)){//if there's not exactly 2 numbers in a line and nothing else
                vecX.push_back(x);
                vecY.push_back(y);
            }
            else{
                fin.close();
                throw InvalidFileData("ERROR: invalid data points found in the file");
            }
        }
    }
    else
    {
        throw FileError("ERROR: file not found / couldn't be opened");
    }
    fin.close();
}
