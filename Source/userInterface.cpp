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
    - initialize a kd tree: -init 
    - insert a new city into the kd tree: -insert [city name]
    - insert from a csv file: -multi_insert [filepath]
    - nearest neighbor search: -nns [city name]
    - range query: -rq [bottomleft] [topright]
    - export result: -save [output]
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

City findCity(const vector<City>& cities, const string& cityName) {
    for (City c : cities) {
        if (c.name == cityName)
            return c;
    }
}

int main() {
    cout << "KD-TREE CHALLENGE\n\n";

    cout << "List of Commands: \n";
    cout << "  - load the list of cities from csv file:     load [filepath]\n";
    cout << "  - initialize a kd tree:                      -init\n";
    cout << "  - insert a new city into the kd tree:        -insert [cityName]\n";
    cout << "  - insert from a csv file:                    -multi_insert [filepath]\n";
    cout << "  - nearest neighbor search:                   -nns [cityName]\n";
    cout << "  - range query:                               -rquery [bottomleft] [topright]\n";
    cout << "  - export result:                             -export [output]\n";

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
            } else {
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
            }
        } else if (method == "insert") {
            if (tokens.size() != 2) {
                cerr << "Usage of '" << method << "': " << method << " [cityName]\n";
            } else {
                string cityName = tokens[1];
                City c = cities[cityName];
                tree.insertKDNode(c);
            }
        } else if (method == "multi_insert") {
            if (tokens.size() != 2) {
                cerr << "Usage of '" << method << "': " << method << " [filePath]\n";
            } else {
                string filePath = tokens[1];
                ifstream ifs(filePath);
                if (!ifs) {
                    cerr << "Unable to open file!\n";
                    continue;
                }
                
                string s;
                while (getline(ifs, s)) {
                    City c = cities[s];
                    tree.insertKDNode(c);
                }
            }
        } else if (method == "nns") {
            if (tokens.size() != 2) {
                cerr << "Usage of '" << method << "': " << method << " [cityName]\n";
            } else {
                cout << "";
            }
        } else if (method == "rquery") {
            if (tokens.size() != 5) {
                cerr << "Usage of '" << method << "': " << method << " [bottomLeft.first] [bottomLeft.second] [topRight.first] [topRight.second]\n";
            } else {
                cout << "";
            }
        } else if (method == "export") {
            if (tokens.size() != 2) {
                cerr << "Usage of '" << method << "': " << method << " [output: filePath/\"cmd\"]\n";
            } else {
                string output = tokens[1];

                if (output == "cmd") {
                    cout << "...";
                } else {
                    ofstream ofs(output);
                    if (!ofs) {
                        cerr << "Unable to open file!\n";
                        continue;
                    }
                    //...
                }
            }
        } else if (method == "quit") {
            exit = true;
        }
    }
}