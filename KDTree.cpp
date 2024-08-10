#include "KDTree.h"
#include "haversine_formula.h"

KDTree::~KDTree()
{
    deleteTreeRec(root);
    root = NULL;
}

void KDTree::deleteTreeRec(KDNode* root)
{
    if (root == NULL)
        return;

    deleteTreeRec(root->left);
    deleteTreeRec(root->right);

    delete root;
}

void KDTree::insertKDNode(const City& newCity)
{
    root = insertKDNodeRec(root, newCity, 0);
}

KDNode* KDTree::insertKDNodeRec (KDNode*& root, const City& newCity, int depth)
{
    if (root == NULL)
    {
        return new KDNode(newCity);
    }

    int curDim = depth % 2; //0 for lat, 1 for long

    if ((curDim == 0 && newCity.coordinate.first < root->base.coordinate.first) ||
        (curDim == 1 && newCity.coordinate.second < root->base.coordinate.second)) 
    {
        root->left = insertKDNodeRec(root->left, newCity, depth + 1);
    } 
    else 
    {
        root->right = insertKDNodeRec(root->right, newCity, depth + 1);
    }

    return root;
}

City KDTree::nearestNeighbour(std::pair<double, double> point)
{
    City res = root->base;
    double minDist = std::numeric_limits<double>::max();
    nearestNeighbourRec(res, root, point, 0, minDist);
    return res;
}

void KDTree::nearestNeighbourRec(City& res, KDNode* root, std::pair<double, double> point, int depth, double minDis)
{
    if (!root)
        return;

    double dist = calculate_distance(point, root->base.coordinate);

    if (dist < minDis)
    {
        minDis = dist;
        res = root->base;
    }

    int curDim = depth % 2;
    KDNode* next = (curDim == 0 && point.first < root->base.coordinate.first) || (curDim == 1 && point.second < root->base.coordinate.second) ? root->left : root->right;
    KDNode* other = (next == root->left) ? root->right : root->left;

    // Explore the next branch (the branch where the target lies)
    nearestNeighbourRec(res, next, point, depth + 1, minDis);

    // Check if we need to explore the other side of the split
    double splitDis = (!curDim) ? fabs(point.first - root->base.coordinate.first) : fabs(point.second - root->base.coordinate.second);

    if (splitDis < minDis) {
        nearestNeighbourRec(res, other, point, depth + 1, minDis);
    }
}

std::vector<City> KDTree::rangeSearch(std::pair<double, double> bottomLeft, std::pair<double,double> topRight)
{
    std::vector<City> res;
    rangeSearchRec(root, bottomLeft, topRight, 0, res);
    return res;
}

void KDTree::rangeSearchRec(KDNode* root, std::pair<double, double> bottomLeft, std::pair<double, double> topRight, int depth, std::vector<City>& res)
{
    if(!root)
    {
        return;
    }

    double nodeLat = root->base.coordinate.first;
    double nodeLong = root->base.coordinate.second;

    if (!(nodeLat < bottomLeft.first || nodeLat > topRight.first || nodeLong < bottomLeft.second || nodeLong > topRight.second))
    {
        res.push_back(root->base);
    }

    int curDim = depth % 2;
    // Determine if we should explore the left subtree
    if ((!curDim && bottomLeft.first <= nodeLat) || (curDim && bottomLeft.second <= nodeLong)) {
        rangeSearchRec(root->left, bottomLeft, topRight, depth + 1, res);
    }

    // Determine if we should explore the right subtree
    if ((!curDim && topRight.first > nodeLat) || (curDim && topRight.second > nodeLong)) {
        rangeSearchRec(root->right, bottomLeft, topRight, depth + 1, res);
    }
}
