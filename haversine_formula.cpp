#include "haversine_formula.h"


/*
    Haversine Formula
    R = earth’s radius (mean radius = 6,371km)
    Δlat = lat2− lat1
    Δlong = long2− long1
    a = sin²(Δlat/2) + cos(lat1).cos(lat2).sin²(Δlong/2)
    c = 2.atan2(√a, √(1−a))
    d = R.c
 */

double calculate_distance(std::pair<double, double> pointA, std::pair<double, double> pointB)
{
    const double earths_radius = 6371.0;

    // Get the difference between our two points then convert the difference into radians
    double lat_delta = convert(pointA.first - pointB.first);
    double lon_delta = convert(pointA.second - pointB.second);

    double converted_lat1 = convert(pointA.first);
    double converted_lat2 = convert(pointB.first);

    double a =
      pow(sin(lat_delta / 2), 2) + cos(converted_lat1) * cos(converted_lat2) * pow(sin(lon_delta / 2), 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double d = earths_radius * c;

    return d;
}

double convert(double angle)
{
    return angle * (M_PI / 180);
}
