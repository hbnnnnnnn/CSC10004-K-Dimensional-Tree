#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <utility>

#include "City.h"
struct KDNode {
    City base;
    KDNode* left, * right;
    KDNode(City a) : base(a), left(NULL), right(NULL){};
};

struct KDTree{
    KDNode* root;
    KDTree() : root(NULL){};

    void insertKDNode(const City& newCity);

    ~KDTree();
    std::vector<City> rangeSearch(std::pair<double, double> bottomLeft, std::pair<double,double> topRight);
    City nearestNeighbour(std::pair<double, double> point);
private:
    KDNode* insertKDNodeRec (KDNode*& root, const City& newCity, int depth);
    void deleteTreeRec(KDNode* root);
    void nearestNeighbourRec(City& res, KDNode* root, std::pair<double, double> point, int depth, double minDis);
    void rangeSearchRec(KDNode* root, std::pair<double, double> bottomLeft, std::pair<double,double> topRight, int depth, std::vector<City>& res);
};
