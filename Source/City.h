#ifndef CITY_H_INCLUDED
#define CITY_H_INCLUDED

#include <utility>
#include <string>
#include <sstream>
struct City{
    std::string name;
    std::pair<double, double> coordinate;
    std::string country;
    long population;
    City extractInfo(std::string line);
};

#endif // CITY_H_INCLUDED
