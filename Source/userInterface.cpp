#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>
#include "City.h"
#include "KDTree.h"

using namespace std;

/* command:
    - load the list of cities from csv file: -load [filepath]
    - insert a new city into the kd tree: -insert [city name]
    - insert from a csv file: -multi_insert [filepath]
    - nearest neighbor search: -nns [city name]
    - range query: -rq [bottomleft] [topright]
    - export result to csv file: -save [output]
*/

vector<string> splitString(const string& s, char delimiter = ' ') {
    stringstream ss(s);
    vector<string> tokens;
    string token;

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

// City findCity(const vector<City>& cities, const string& cityName) {
//     for (City c : cities) {
//         if (c.name == cityName)
//             return c;
//     }
// }

ostream& operator<<(ostream& os, const City& c) {
    os << c.name;
    return os;
}

bool validCity(map<string, City>& cities, const string& cityName) {
    if (cities.find(cityName) != cities.end())
        return true;
    return false;
}

int main() {
    cout << "KD-TREE CHALLENGE\n\n";

    cout << "List of Commands: \n";
    cout << "  - load the list of cities from csv file:     load [filepath]\n";
    cout << "  - insert a new city into the kd tree:        insert [cityName]\n";
    cout << "  - insert from a csv file:                    multi_insert [filepath]\n";
    cout << "  - nearest neighbor search:                   nns [cityName]\n";
    cout << "  - range query:                               rquery [bottomleft.x] [bottomleft.y] [topright.x] [topright.y]\n";
    cout << "  - save result to csv file:                   save [filePath]\n";
    cout << "  - terminate program:                         quit\n\n";
    cout << "Main program:\n\n";

    KDTree tree;
    map<string, City> cities;
    bool exit = false;
    string command;

    while (!exit) {
        cout << "New command: ";
        getline(cin, command);
        vector<string> tokens = splitString(command);
        string method = tokens[0];

        // Handling "load" command
        if (method == "load") {
            if (tokens.size() != 2) {
                cerr << "Usage of '" << method << "': " << method << " [filePath]\n";
            }
            else {
                string filePath = tokens[1];
                ifstream ifs(filePath);
                if (!ifs) {
                    cerr << "Unable to open file!\n";
                    continue;
                }

                string s;
                City c;
                while (getline(ifs, s)) {
                    c = c.extractInfo(s);
                    cities[c.name] = c;
                }

                cout << "Loaded cities into database successfully!\n";
            }
        }
        else if (method == "insert") {
            if (tokens.size() != 2) {
                cerr << "Usage of '" << method << "': " << method << " [cityName]\n";
            }
            else {
                string cityName = tokens[1];
                if (cities.find(cityName) != cities.end()) {
                    City c = cities[cityName];
                    tree.insertKDNode(c);
                    cout << cityName << " has been inserted.\n";
                }
                else {
                    cerr << cityName << " not found in the database.\n";
                }
            }
        }
        else if (method == "multi_insert") {
            if (tokens.size() != 2) {
                cerr << "Usage of '" << method << "': " << method << " [filePath]\n";
                continue;
            }
            
            string filePath = tokens[1];
            ifstream ifs(filePath);
            if (!ifs) {
                cerr << "Unable to open file!\n";
                continue;
            }

            string s;
            while (getline(ifs, s)) {
                if (cities.find(s) != cities.end()) {
                    City c = cities[s];
                    tree.insertKDNode(c);
                    cout << s << " has been inserted.\n";
                }
                else {
                    cerr << s << " not found in the database.\n";
                }
            }
        }
        else if (method == "nns") {
            if (tokens.size() != 2) {
                cerr << "Usage of '" << method << "': " << method << " [cityName]\n";
                continue;
            }
            
            string cityName = tokens[1];
            if (!validCity(cities, cityName)) { // sua lai la kiem tra city co trong tree ch
                cerr << "City not found in current tree.\n";
                continue;
            }

            City c = cities[cityName];
            City nearestNeighbor = tree.nearestNeighbour(c.coordinate);
            cout << "Nearest neighbor found: " << nearestNeighbor.name << "\n";
        }
        else if (method == "rquery") {
            if (tokens.size() != 5) {
                cerr << "Usage of '" << method << "': " << method << " [bottomLeft.first] [bottomLeft.second] [topRight.first] [topRight.second]\n";
                continue;
            }
            
            pair<int, int> bottomLeft = {stoi(tokens[1]), stoi(tokens[2])};
            pair<int, int> topRight = {stoi(tokens[3]), stoi(tokens[4])};
            vector<City> rangeQuery = tree.rangeSearch(bottomLeft, topRight);
            for (City i : rangeQuery) {
                cout << i << "\n";
            }
        }
        else if (method == "save") {
            if (tokens.size() != 2) {
                cerr << "Usage of '" << method << "': " << method << " [filePath]\n";
                continue;
            }
            
            string filePath = tokens[1];

            ofstream ofs(filePath);
            if (!ofs) {
                cerr << "Unable to open file!\n";
                continue;
            }
            // luu kq vao csv
        }
        else if (method == "quit") {
            cout << "Exiting...\n";
            exit = true;
        }
    }
}
