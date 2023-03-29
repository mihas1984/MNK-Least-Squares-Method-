#include "mnk.hpp"
#include <vector>

void mnk1(std::vector<int>& x, std::vector<int>& y, double& a1, double& b1)
{
    double sumy = 0;
    double sumx = 0;
    double sumx2 = 0;
    double sumyx = 0;
    for (int i = 0; i < y.size(); i++)
    {
        sumy += y.at(i);
    }
    for (int i = 0; i < x.size(); i++)
    {
        sumx += x.at(i);
    }
    for (int i = 0; i < x.size(); i++)
    {
        sumx2 += x.at(i)*x.at(i);
    }
    for (int i = 0; i < x.size(); i++)
    {
        sumyx += x.at(i)*y.at(i);
    }
    a1 = (x.size() * sumyx - sumx * sumy)/ (x.size() * sumx2 - sumx * sumx);
    b1 = (sumy - a1 * sumx) / x.size();
}
