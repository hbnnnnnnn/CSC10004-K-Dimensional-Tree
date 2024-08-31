#ifndef HARVERSINE_FORMULA_H_INCLUDED
#define HARVERSINE_FORMULA_H_INCLUDED

#define _USE_MATH_DEFINES
#include <cmath>
#include <utility>

double calculate_distance(std::pair<double, double> pointA, std::pair<double, double> pointB);

double convert(double angle);

#endif // HARVERSINE_FORMULA_H_INCLUDED
