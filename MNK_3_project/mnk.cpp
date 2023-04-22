#include "mnk.hpp"
#include <vector>
#include <string>

int computate(const std::vector<double> x, const std::vector<double> y, double& a, double& b, std::string& error)
{
    double sumy = 0;// sum of all y values
    double sumx = 0;// sum of all x values
    double sumx2 = 0;// sum of all x*x values
    double sumyx = 0;// sum of all y*x values
    for (int i = 0; i < (int)y.size(); i++)
    {
        sumy += y.at(i);
    }
    for (int i = 0; i < (int)x.size(); i++)
    {
        sumx += x.at(i);
    }
    for (int i = 0; i < (int)x.size(); i++)
    {
        sumx2 += x.at(i)*x.at(i);
    }
    for (int i = 0; i < (int)x.size(); i++)
    {
        sumyx += x.at(i)*y.at(i);
    }
    if(!(x.size())){
        error = "ERROR: zero data points found";
        return -1;
    }else if(!(x.size() * sumx2 - sumx * sumx)){
        error = "ERROR: dividing by zero (data cannot be fitted)";
        return -1;
    }
    a = (x.size() * sumyx - sumx * sumy)/ (x.size() * sumx2 - sumx * sumx);
    b = (sumy - a * sumx) / x.size();
    return 0;
}
