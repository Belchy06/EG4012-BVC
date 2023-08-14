#include "haar.h"

double				haar::inv_sqrt2 = 1.0 / std::sqrt(2.0);
std::vector<double> haar::lo_d = { inv_sqrt2, inv_sqrt2 };
std::vector<double> haar::hi_d = { -inv_sqrt2, inv_sqrt2 };
std::vector<double> haar::lo_r = { inv_sqrt2, inv_sqrt2 };
std::vector<double> haar::hi_r = { inv_sqrt2, -inv_sqrt2 };