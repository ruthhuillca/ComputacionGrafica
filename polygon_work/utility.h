#ifndef UTILITY_H
#define UTILITY_H

#include <QDebug>

#include <vector>
#include <algorithm>

#include <polygon.h>

#define NORMAL_NODE 0
#define IN_NODE 1
#define OUT_NODE 2

#define P_TABLE 0
#define Q_TABLE 1

using namespace std;

struct LinkNode {
    int tableId;
    int vertexId;
    int type;
    int x;
    int y;
    double r;
    LinkNode *next;
    LinkNode *equal;
    LinkNode(int tId, int vId, int t) : tableId(tId), vertexId(vId), type(t) {}
    LinkNode(int xCor, int yCor, double rate, int t) : x(xCor), y(yCor), r(rate), type(t) {}
};

bool compare(LinkNode *p, LinkNode *q);

class Utility {
public:
    vector<Polygon> WeilerAtherton(Polygon &p, Polygon &q);

private:
    bool isClockWise(vector<pair<int, int>> &nodes);
    vector<LinkNode *> createTable(Polygon &p);
    void insertNodes(vector<LinkNode *> &pTable_, vector<LinkNode *> &qTable_,
                     vector<LinkNode *> &pTable, vector<LinkNode *> &qTable,
                     Polygon &p, Polygon &q);
    bool isCross(pair<int, int> &p1, pair<int, int> &p2, pair<int, int> &q1, pair<int, int> &q2,
                 double &r1, double &r2, double &x, double &y);
    double crossProduct(pair<int, int> &p1, pair<int, int> &p2,
                        pair<int, int> &q1, pair<int, int> &q2);
    double getArea(Polygon &p);
    bool isPointInPolygon(pair<int, int> &point, Polygon &p);
    double angleBetween(double x1, double y1, double x2, double y2);

public:
    Utility();
};

#endif // UTILITY_H
