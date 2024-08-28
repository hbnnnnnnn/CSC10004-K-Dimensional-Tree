#include "City.h"

City City::extractInfo(std::string line) {
    std::stringstream ss(line);
    City res;
    char tmp;
    std::getline(ss, res.name, ',');
    ss >> res.coordinate.second; //lat
    ss >> tmp;
    ss >> res.coordinate.first; //long
    ss >> tmp;
    std::getline(ss, res.country, ',');
    ss >> res.population;
    return res;
}
