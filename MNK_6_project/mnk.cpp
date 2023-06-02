#include "mnk.hpp"
#include "exceptions.hpp"
#include <vector>
#include <string>
#include <QtMath>

int computate_equation(const std::vector<long double> x, const std::vector<long double> y, long double& slope, long double& intercept)
{
    long double sumy {};// sum of all y values
    long double sumx {};// sum of all x values
    long double sumx2 {};// sum of all x*x values
    long double sumyx {};// sum of all y*x values
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
    if(x.size() < 2){
        throw InvalidComputation("ERROR: less than two data points found");
    }
    if(!(x.size() * sumx2 - sumx * sumx)){
        slope = 0;
        intercept = sumx/(double)x.size();
        throw ComputationWarning("slope is equal to zero. Data points form a vertical line.");
    }
    slope = (x.size() * sumyx - sumx * sumy)/ (x.size() * sumx2 - sumx * sumx);
    intercept = (sumy - slope * sumx) / x.size();
    return 0;
}

int computate_equation_error(const std::vector<long double> x, const std::vector<long double> y, const long double slope,
                             const long double intercept, long double& slope_error, long double& intercept_error){
    long double sum_y_diff_2 {};//sum of (y.at(i) - y_that_should_be.at(i))^2 //y_that_should_be.at(i) = slope*x.at(i)+intercept
    long double sum_x_diff_2 {};//sum of (x.at(i) - avg_x)^2
    long double avg_x {};//arithmetic mean of x
    for(int i = 0; i < (int)x.size(); i++){
        avg_x += x.at(i);
    }
    avg_x /= (double)x.size();

    for(int i = 0; i < (int)x.size(); i++){
        sum_x_diff_2 += (x.at(i) - avg_x) * (x.at(i) - avg_x);
    }

    for(int i = 0; i < (int)x.size(); i ++){
        sum_y_diff_2 += (y.at(i) - (slope * x.at(i) + intercept)) * (y.at(i) - (slope * x.at(i) + intercept));
    }

    if((x.size() == 2) || (sum_y_diff_2 == 0) || (sum_x_diff_2 == 0)){
        return -1;
    }

    intercept_error = qSqrt(sum_y_diff_2/(x.size()-2)) * qSqrt(1.0/x.size() + avg_x*avg_x/sum_x_diff_2);
    slope_error = qSqrt(sum_y_diff_2/(x.size()-2)/sum_x_diff_2);
    return 0;
}
