#ifndef KDTREE_H_INCLUDED
#define KDTREE_H_INCLUDED

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
    ~KDTree();
    void insertKDNode(const City& newCity);
    std::vector<City> rangeSearch(std::pair<double, double> bottomLeft, std::pair<double,double> topRight);
    City nearestNeighbour(std::pair<double, double> point);
private:
    void deleteTreeRec(KDNode* root);
    KDNode* insertKDNodeRec (KDNode*& root, const City& newCity, int depth);
    void rangeSearchRec(KDNode* root, std::pair<double, double> bottomLeft, std::pair<double,double> topRight, int depth, std::vector<City>& res);
    void nearestNeighbourRec(City& res, KDNode* root, std::pair<double, double> point, int depth, double minDis);
};

#endif // KDTREE_H_INCLUDED
