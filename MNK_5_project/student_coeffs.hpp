#ifndef STUDENT_COEFFS_HPP
#define STUDENT_COEFFS_HPP

double student_coeffs[1+30+4] = {
    0,
    12.7,4.3,3.18,2.78,2.57,
    2.45,2.36,2.31,2.26,2.23,
    2.2,2.18,2.16,2.14,2.13,
    2.12,2.11,2.10,2.09,2.09,
    2.08,2.07,2.07,2.06,2.06,
    2.06,2.05,2.05,2.05,2.04,
    2.02,//31 - 40
    2.00,//41 - 60
    1.98,//61 - 120
    1.96//120+
};//index corresponding to count of data points

#endif // STUDENT_COEFFS_HPP
