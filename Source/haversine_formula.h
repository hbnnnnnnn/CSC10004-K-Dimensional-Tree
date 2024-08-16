#ifndef HARVERSINE_FORMULA_H
#define HARVERSINE_FORMULA_H

#include <numbers>
#include <utility>
#include <cmath>

double calculate_distance(std::pair<double, double> pointA, std::pair<double, double> pointB);

double convert(double angle);

#endif
