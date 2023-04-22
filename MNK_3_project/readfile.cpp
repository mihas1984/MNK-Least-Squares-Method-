#include "readfile.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

int readData(std::vector<double>& vecX, std::vector<double>& vecY, std::ifstream& fin, std::string& error){
    if (fin)
    {
        std::stringstream ss;
        ss << fin.rdbuf();
        std::string line;
        while (std::getline(ss, line)) {//splitting data line by line
            std::stringstream lineStream(line);
            double x {0};
            double y {0};
            std::string other { "" };
            if((lineStream >> x >> y) && !(lineStream >> other)){//if there's not exactly 2 numbers in a line and nothing else
                vecX.push_back(x);
                vecY.push_back(y);
            }
            else{
                error = "ERROR: invalid data in the file";
                fin.close();
                return -1;
            }
        }
    }
    else
    {
        error = "ERROR: file not found / couldn't be opened";
        fin.close();
        return -1;
    }
    fin.close();
    return 0;
}
