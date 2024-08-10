#include <utility>
#include <string>
#include <sstream>
struct City{
    std::string name;
    std::pair<double, double> coordinate;
    std::string country;
    long population;
};

City extractInfo(std::string line){
    std::stringstream ss(line);
    City res;
    char tmp;
    std::getline(ss, res.name, ',');
    ss >> res.coordinate.first;
    ss >> tmp;
    ss >> res.coordinate.second;
    ss >> tmp;
    std::getline(ss, res.country, ',');
    ss >> res.population;
    return res;
}
