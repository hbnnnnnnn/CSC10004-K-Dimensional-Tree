#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>
#include "City.h"
#include "KDTree.h"

using namespace std;

vector<string> splitString(const string& s) {
    stringstream ss(s);
    vector<string> tokens;
    string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

ostream& operator<<(ostream& os, const City& c) {
    os << c.name;
    return os;
}

bool cityInDatabase(map<string, City>& cities, const string& cityName) {
    if (cities.find(cityName) != cities.end())
        return true;
    return false;
}

int main() {
    map<string, string> usage;
    usage["load"] = "load [.csv file]";
    usage["rcs"] = "rcs [.bin file]";
    usage["insert"] = "insert [city name]";
    usage["multi_insert"] = "multi_insert [.csv file]";
    usage["nns"] = "nns [latitude] [longitude]";
    usage["rquery"] = "rquery [bottomleft.long] [bottomleft.lat] [topright.long] [topright.lat]";
    usage["save"] = "save [.bin file]";
    usage["quit"] = "quit";

    string commandPadding = "  ";
    string messagePadding = "    -> ";

    cout << "KD-TREE CHALLENGE\n\n";

    cout << "List of Commands: \n\n";
    cout << "  - load the list of cities from csv file:     " << usage["load"] << "\n";
    cout << "  - reconstruct from existing file:            " << usage["rcs"] << "\n";
    cout << "  - insert a new city into the kd tree:        " << usage["insert"] << "\n";
    cout << "  - insert from a csv file:                    " << usage["multi_insert"] << "\n";
    cout << "  - nearest neighbor search:                   " << usage["nns"] << "\n";
    cout << "  - range query:                               " << usage["rquery"] << "\n";
    cout << "  - save result to csv file:                   " << usage["save"] << "\n";
    cout << "  - terminate program:                         " << usage["quit"] << "\n\n";
    cout << "Main program:\n\n";

    KDTree tree;
    map<string, City> cities;
    bool exit = false;
    string command;
    bool csv = false;
    string outputFile;
    
    cout << commandPadding + "Specify a .csv file if you wish to output your results to one? This includes any results related to Nearest Neighbor Query or Range Query. Otherwise, type anything to proceed: ";
    getline(cin, command);
    int length = command.length();

    if (length > 4) {
        string lastFour = command.substr(length - 4, 4);
        vector<string> tokens = splitString(command);

        if (lastFour == ".csv" && tokens.size() == 1) {
            string filePath = command;
            ofstream ofs(filePath);

            if (!ofs) {
                cout << messagePadding + "Unable to open file!\n";
            }
            else {
                csv = true;
                outputFile = filePath;
            }

            ofs.close();
        }
    }

    if (!csv) {
        cout << messagePadding + "Results will be displayed to the console.\n\n";
    }
    else {
        cout << messagePadding + "Results will be saved to the .csv file.\n\n";
    }
    
    while (!exit) {
        cout << commandPadding + "New command: ";
        getline(cin, command);
        vector<string> tokens = splitString(command);
        string method = tokens[0];
        transform(method.begin(), method.end(), method.begin(), ::tolower);

        if (method == "load") {
            if (tokens.size() != 2) {
                cout << messagePadding + "Usage of '" << method << "': " << usage[method] << "\n\n";
            }
            else {
                string filePath = tokens[1];
                ifstream ifs(filePath);
                if (!ifs) {
                    cout << messagePadding + "Unable to open file!\n";
                    continue;
                }

                string s;
                City c;
                while (getline(ifs, s)) {
                    c = c.extractInfo(s);
                    cities[c.name] = c;
                }

                cout << messagePadding + "Database built successfully!\n\n";
            }
        }
        else if (method == "rcs") {
            if (tokens.size() != 2) {
                cout << messagePadding + "Usage of '" << method << "': " << usage[method] << "\n\n";
                continue;
            }

            if (tree.root) {
                cout << messagePadding + "Recontruction only possible with an empty tree.\n\n";
                continue;
            }

            string filePath = tokens[1];
            tree.deserialize(filePath);

            cout << messagePadding + "Reconstructed successfully.\n\n";
        }
        else if (method == "insert") {
            if (tokens.size() < 2) {
                cout << messagePadding + "Usage of '" << method << "': " << usage[method] << "\n\n";
                continue;
            }

            if (cities.size() == 0) {
                cout << messagePadding + "Database found to be empty. Load it before proceeding.\n\n";
                continue;
            }

            string cityName;
            int n = tokens.size();
            for (int i = 1; i < n; i++) {
                cityName += (i == 1 ? "" : " ") + tokens[i];
            }

            if (cities.find(cityName) != cities.end()) {
                City c = cities[cityName];
                if (tree.inKDTree(c)) {
                    cout << messagePadding + cityName + " has already been in the tree.\n\n";
                    continue;
                }
                tree.insertKDNode(c);
                cout << messagePadding + cityName << " inserted.\n\n";
            }
            else {
                cout << messagePadding + cityName << " not found in the database.\n\n";
            }
        }
        else if (method == "multi_insert") {
            if (tokens.size() != 2) {
                cout << messagePadding + "Usage of '" << method << "': " << usage[method] << "\n\n";
                continue;
            }

            if (cities.size() == 0) {
                cout << messagePadding + "Database found to be empty. Load it before proceeding.\n\n";
                continue;
            }

            string filePath = tokens[1];
            ifstream ifs(filePath);
            if (!ifs) {
                cout << messagePadding + "Unable to open file!\n\n";
                continue;
            }

            string s;
            while (getline(ifs, s)) {
                if (cities.find(s) != cities.end()) {
                    City c = cities[s];
                    if (tree.inKDTree(c)) {
                        cout << messagePadding + s + " has already been in the tree.\n";
                        continue;
                    }
                    tree.insertKDNode(c);
                    cout << messagePadding + s << " has been inserted.\n";
                }
                else {
                    cout << messagePadding + s << " not found in the database.\n";
                }
            }

            cout << "\n";
        }
        else if (method == "nns") {
            if (tokens.size() != 3) {
                cout << messagePadding + "Usage of '" << method << "': " << usage[method] << "\n\n";
                continue;
            }

            if (!tree.root) {
                cout << messagePadding + "Tree void of cities, please insert some before performing Nearest Neighbor Query.\n\n";
                continue;
            }

            double latitude = stod(tokens[1]);
            double longitude = stod(tokens[2]);

            pair<City, double> nearestNeighbor = tree.nearestNeighbour({longitude, latitude});

            if (!csv) {
                cout << messagePadding + "Nearest neighbor found: " << nearestNeighbor.first.name << ".\n";
                cout << messagePadding + "Distance: " << nearestNeighbor.second << "km.\n\n";
            }
            else {
                ofstream ofs(outputFile, ios::app);
                ofs << "Nearest neighbor from <"  << latitude << ", " << longitude << "> found: " << nearestNeighbor.first.name << ".\n";
                ofs << "Distance: " << nearestNeighbor.second << "km.\n\n";
                ofs.close();

                cout << messagePadding + "Results saved to .csv file.\n\n";
            }
            
        }
        else if (method == "rquery") {
            if (tokens.size() != 5) {
                cout << messagePadding + "Usage of '" << method << "': " << usage[method] << "\n\n";
                continue;
            }

            pair<double, double> bottomLeft = {stod(tokens[1]), stod(tokens[2])};
            pair<double, double> topRight = {stod(tokens[3]), stod(tokens[4])};

            vector<City> rangeQuery = tree.rangeSearch(bottomLeft, topRight);
            if (rangeQuery.size() == 0) {
                cout << messagePadding + "No cities found in this range.\n\n";
                continue;
            }

            if (!csv) {
                cout << messagePadding + "Result of range search: ";
                for (auto it = rangeQuery.begin(); it != rangeQuery.end(); it++) {
                    if (it != rangeQuery.begin()) {
                        cout << ", ";
                    }
                    cout << *it;
                }
                cout << ".\n\n";
            }
            else {
                ofstream ofs(outputFile, ios::app);
                ofs << "Result of range search <" << bottomLeft.first << ", " << bottomLeft.second << "> <" << topRight.first << ", " << topRight.second << ">: ";
                for (auto it = rangeQuery.begin(); it != rangeQuery.end(); it++) {
                    if (it != rangeQuery.begin()) {
                        ofs << ", ";
                    }
                    ofs << *it;
                }
                ofs << ".\n\n";
                ofs.close();

                cout << messagePadding + "Results saved to .csv file.\n\n";
            }
            
        }
        else if (method == "save") {
            if (tokens.size() != 2) {
                cout << messagePadding + "Usage of '" << method << "': " << usage[method] << "\n\n";
                continue;
            }

            string filePath = tokens[1];

            ofstream ofs(filePath);
            if (!ofs) {
                cout << messagePadding + "Unable to open file!\n\n";
                continue;
            }

            tree.serialize(filePath);
            cout << messagePadding + "Tree saved.\n\n";
        }
        else if (method == "quit") {
            cout << messagePadding + "Exiting...\n";
            exit = true;
        }
        else {
            cout << messagePadding + "Invalid command.\n\n";
        }
    }

    return 0;
}
