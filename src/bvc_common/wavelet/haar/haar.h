#pragma once

#include <cmath>
#include <vector>

class haar
{
public:
	static std::vector<double> lo_d;
	static std::vector<double> hi_d;
	static std::vector<double> lo_r;
	static std::vector<double> hi_r;

private:
	static double inv_sqrt2;
};