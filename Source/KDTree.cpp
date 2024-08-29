#include "KDTree.h"
#include "haversine_formula.h"

KDTree::~KDTree()
{
    deleteTreeRec(root);
    root = NULL;
}

void KDTree::deleteTreeRec(KDNode* root)
{
    if (!root)
    {
        return;
    }

    deleteTreeRec(root->left);
    deleteTreeRec(root->right);

    delete root;
}

void KDTree::insertKDNode(const City& newCity)
{
    root = insertKDNodeRec(root, newCity, 0);
}

bool KDTree::inKDTree(City& city)
{
    return inKDTreeRec(root, city, 0);
}

bool KDTree::inKDTreeRec(KDNode* root, City& city, int depth)
{
    if (!root)
    {
        return false;
    }

    if (root->base.coordinate == city.coordinate)
    {
        return true;
    }

    int curDim = depth % 2;

    if ((!curDim && city.coordinate.first < root->base.coordinate.first) ||
        (curDim && city.coordinate.second < root->base.coordinate.second))
    {
        return inKDTreeRec(root->left, city, depth + 1);
    }

    return inKDTreeRec(root->right, city, depth + 1);
}

KDNode* KDTree::insertKDNodeRec(KDNode*& root, const City& newCity, int depth)
{
    if (!root)
    {
        return new KDNode(newCity);
    }

    int curDim = depth % 2; //0 for long, 1 for lat

    if ((!curDim && newCity.coordinate.first < root->base.coordinate.first) ||
        (curDim && newCity.coordinate.second < root->base.coordinate.second))
    {
        root->left = insertKDNodeRec(root->left, newCity, depth + 1);
    }

    else if ((!curDim && newCity.coordinate.first > root->base.coordinate.first) ||
        (curDim && newCity.coordinate.second > root->base.coordinate.second))
    {
        root->right = insertKDNodeRec(root->right, newCity, depth + 1);
    }

    return root;
}

std::pair<City, double> KDTree::nearestNeighbour(std::pair<double, double> point)
{
    City res = root->base;
    double minDist = std::numeric_limits<double>::max();
    nearestNeighbourRec(res, root, point, 0, minDist);
    return {res, minDist};
}

void KDTree::nearestNeighbourRec(City& res, KDNode* root, std::pair<double, double> point, int depth, double& minDis)
{
    if (!root)
    {
        return;
    }

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
    
    if (splitDis < minDis)
    {
        nearestNeighbourRec(res, other, point, depth + 1, minDis);
    }
}

std::vector<City> KDTree::rangeSearch(std::pair<double, double> bottomLeft, std::pair<double, double> topRight)
{
    std::vector<City> res;
    rangeSearchRec(root, bottomLeft, topRight, 0, res);
    return res;
}

void KDTree::rangeSearchRec(KDNode* root, std::pair<double, double> bottomLeft, std::pair<double, double> topRight, int depth, std::vector<City>& res)
{
    if (!root)
    {
        return;
    }

    double nodeLong = root->base.coordinate.first;
    double nodeLat = root->base.coordinate.second;

    if (!(nodeLong < bottomLeft.first || nodeLong > topRight.first || nodeLat < bottomLeft.second || nodeLat > topRight.second))
    {
        res.push_back(root->base);
    }

    int curDim = depth % 2;
    // Determine if we should explore the left subtree
    if ((!curDim && bottomLeft.first <= nodeLong) || (curDim && bottomLeft.second <= nodeLat))
    {
        rangeSearchRec(root->left, bottomLeft, topRight, depth + 1, res);
    }

    // Determine if we should explore the right subtree
    if ((!curDim && topRight.first > nodeLong) || (curDim && topRight.second > nodeLat))
    {
        rangeSearchRec(root->right, bottomLeft, topRight, depth + 1, res);
    }
}

std::vector<City> KDTree::levelOrder(KDNode* root) {
    std::vector<City> ans;
    if (root == NULL) {
        return ans;
    }
    City null;
    null.name = "empty";
    null.coordinate = {0, 0};
    null.country = "empty";
    null.population = 0;

    std::queue<KDNode*> q;
    q.push(root);
    while (!q.empty()) {
        KDNode* top = q.front();
        q.pop();
        if (top == NULL) {
            ans.push_back(null);
        } else {
            ans.push_back(top->base);
            q.push(top->left);
            q.push(top->right);
        }
    }
    return ans;
}

void KDTree::serialize(const std::string &filename) {
    std::ofstream ofs;
    ofs.open(filename, std::ios::binary);
    if (!ofs) {
        std::cerr << "Error opening file for write";
        return;
    }
    std::vector<City> city = levelOrder(root);
    size_t size = city.size();
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const City &c : city) {
        // write length of name and name
        size_t nameLength = c.name.size();
        ofs.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        ofs.write(c.name.c_str(), nameLength);
        // write coordinate
        ofs.write(reinterpret_cast<const char*>(&c.coordinate.first), sizeof(c.coordinate.first));
        ofs.write(reinterpret_cast<const char*>(&c.coordinate.second), sizeof(c.coordinate.second));
        // write length of country name and country name
        size_t countryLength = c.country.size(); // Length of country name
        ofs.write(reinterpret_cast<const char*>(&countryLength), sizeof(countryLength));
        ofs.write(c.country.c_str(), countryLength);
        // write population
        ofs.write(reinterpret_cast<const char*>(&c.population), sizeof(c.population));
    }
    return;
}

KDNode* KDTree::getTree(std::vector<City> city) {
    int size = city.size();
    if (size == 0) {
        return NULL;
    }
    std::queue<KDNode*> q;
    KDNode* root = new KDNode(city[0]);
    q.push(root);
    int pointer = 1;
    while (!q.empty() && pointer < size) {
        KDNode* top = q.front();
        q.pop();
        if (pointer < size && city[pointer].name == "empty") {
            top->left = NULL;
        } else if (pointer < size) {
            KDNode* nodeLeft = new KDNode(city[pointer]);
            top->left = nodeLeft;
            q.push(nodeLeft);
        }
        pointer++;
        if (pointer < size && city[pointer].name == "empty") {
            top->right = NULL;
        } else if (pointer < size) {
            KDNode* nodeRight = new KDNode(city[pointer]);
            top->right = nodeRight;
            q.push(nodeRight);
        }
        pointer++;
    }
    return root;
}

void KDTree::deserialize(const std::string &filename) {
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    if (!ifs) {
        std::cerr << "Error opening file";
        return;
    }
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::vector<City> city(size);
    for (City &c : city) {
        // Read length of name and name
        size_t nameLength;
        ifs.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        c.name.resize(nameLength);
        ifs.read(&c.name[0], nameLength);

        // Read coordinate
        ifs.read(reinterpret_cast<char*>(&c.coordinate.first), sizeof(c.coordinate.first));
        ifs.read(reinterpret_cast<char*>(&c.coordinate.second), sizeof(c.coordinate.second));

        // Read length of country name and country name
        size_t countryLength;
        ifs.read(reinterpret_cast<char*>(&countryLength), sizeof(countryLength));
        c.country.resize(countryLength);
        ifs.read(&c.country[0], countryLength);

        // Read population
        ifs.read(reinterpret_cast<char*>(&c.population), sizeof(c.population));
    }
    root = getTree(city);
    
}
