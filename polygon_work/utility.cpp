#include "utility.h"

Utility::Utility() {

}

vector<Polygon> Utility::WeilerAtherton(Polygon &p, Polygon &q) {
    vector<Polygon> polygons;
    // Create Vertex Table
    vector<LinkNode *> mainTableOrigin = createTable(p);
    vector<LinkNode *> cutTableOrigin = createTable(q);
    // Insert Nodes
    vector<LinkNode *> mainTable;
    vector<LinkNode *> cutTable;
    insertNodes(mainTableOrigin, cutTableOrigin, mainTable, cutTable, p, q);
    // The special case: no cross point
    if (mainTable.size() == mainTableOrigin.size()) {
        if (getArea(p) < getArea(q)) {
            Polygon newP = p;
            polygons.push_back(newP);
            return polygons;
        } else {
            Polygon newP;
            newP.addOutPoints(q.vertex[0]);
            for (int i = 1; i < p.vertex.size(); ++i) {
                bool isInPolygon = isPointInPolygon(p.vertex[i][0], q);
                if (isInPolygon) {
                    newP.addInPoints(p.vertex[i]);
                }
            }
            polygons.push_back(newP);
            return polygons;
        }
    }
    // Search the points
    while (true) {
        Polygon newP;
        vector<pair<int, int>> points;
        bool isFound = false;
        LinkNode *pNode = NULL;
        for (int i = 0; i < mainTable.size(); ++i) {
            if (mainTable[i]->type == IN_NODE) {
                isFound = true;
                pNode = mainTable[i];
                break;
            }
        }
        if (!isFound || pNode == NULL) {
            break;
        }
        pNode->type = NORMAL_NODE;
        int currentTable = P_TABLE;
        points.push_back(make_pair(pNode->x, pNode->y));
        LinkNode *head = pNode;
        while (true) {
            if (currentTable == P_TABLE) {
                pNode = pNode->next;
                if (pNode->type == NORMAL_NODE) {
                    points.push_back(p.vertex[pNode->tableId][pNode->vertexId]);
                } else if (pNode->type == IN_NODE) {
                    points.push_back(make_pair(pNode->x, pNode->y));
                } else {
                    points.push_back(make_pair(pNode->x, pNode->y));
                    pNode = pNode->equal;
                    currentTable = Q_TABLE;
                }
            } else {
                pNode = pNode->next;
                if (pNode->type == NORMAL_NODE) {
                    points.push_back(p.vertex[pNode->tableId][pNode->vertexId]);
                } else if (pNode->type == OUT_NODE) {
                    points.push_back(make_pair(pNode->x, pNode->y));
                } else {
                    pNode = pNode->equal;
                    currentTable = P_TABLE;
                    if (pNode == head) {
                        break;
                    } else {
                        pNode->type = NORMAL_NODE;
                        points.push_back(make_pair(pNode->x, pNode->y));
                    }
                }
            }
        }
        newP.addOutPoints(points);
        polygons.push_back(newP);
    }
    return polygons;
}

bool Utility::isClockWise(vector<pair<int, int> > &nodes) {
    int area = 0;
    for (int i = 0; i < nodes.size() - 1; ++i) {
        area += nodes[i].first * nodes[i + 1].second;
        area -= nodes[i + 1].first * nodes[i].second;
    }
    area += nodes[nodes.size() - 1].first * nodes[0].second;
    area -= nodes[0].first * nodes[nodes.size() - 1].second;
    return (area < 0);
}

vector<LinkNode *> Utility::createTable(Polygon &p) {
    vector<LinkNode *> table;
    for (int i = 0; i < p.vertex.size(); ++i) {
        vector<pair<int, int>> nodes = p.vertex[i];
        bool isClockwise = isClockWise(nodes);
        bool isReverse = ((i == 0 && !isClockwise) || (i != 0 && isClockwise));
        //bool isReverse = false;
        LinkNode *last = NULL, *head = NULL;
        for(int j = 0; j < nodes.size(); ++j) {
            LinkNode *pNode = new LinkNode(i, (isReverse ? (int(nodes.size()) - j - 1) : j), NORMAL_NODE);
            if (head == NULL) {
                head = pNode;
            }
            if (last != NULL) {
                last->next = pNode;
            }
            last = pNode;
            table.push_back(pNode);
        }
        last->next = head;
    }
    return table;
}

bool Utility::isCross(pair<int, int> &p1, pair<int, int> &p2, pair<int, int> &q1, pair<int, int> &q2,
                      double &r1, double &r2, double &x, double &y) {
    double x1 = p1.first, y1 = p1.second;
    double x2 = p2.first, y2 = p2.second;
    double x3 = q1.first, y3 = q1.second;
    double x4 = q2.first, y4 = q2.second;
    double d = (x2 - x1) * (y4 - y3) - (x4 - x3) * (y2 - y1);
    if (d == 0) {
        return false;
    }
    double b1 = (y2 - y1) * x1 + (x1 - x2) * y1;
    double b2 = (y4 - y3) * x3 + (x3 - x4) * y3;
    double d1 = b2 * (x2 - x1) - b1 * (x4 - x3);
    double d2 = b2 * (y2 - y1) - b1 * (y4 - y3);
    x = d1 / d;
    y = d2 / d;
    if (p1.first == p2.first) {
        r1 = (y - y1) / (y2 - y1);
    } else {
        r1 = (x - x1) / (x2 - x1);
    }
    if (q1.first == q2.first) {
        r2 = (y - y3) / (y4 - y3);
    } else {
        r2 = (x - x3) / (x4 - x3);
    }
    if (r1 > 0.0 && r1 < 1.0 && r2 > 0.0 && r2 < 1.0) {
        return true;
    }
    return false;
}

bool compare(LinkNode *p, LinkNode *q) {
    return (p->r < q->r);
}

double Utility::crossProduct(pair<int, int> &p1, pair<int, int> &p2,
                             pair<int, int> &q1, pair<int, int> &q2) {
    double x1 = p2.first - p1.first;
    double x2 = q2.first - q1.first;
    double y1 = p2.second - p1.second;
    double y2 = q2.second - q1.second;
    return x1 * y2 - x2 * y1;
}

void Utility::insertNodes(vector<LinkNode *> &pTable_, vector<LinkNode *> &qTable_,
                          vector<LinkNode *> &pTable, vector<LinkNode *> &qTable,
                          Polygon &p, Polygon &q) {
    vector<vector<LinkNode *>> pNodes;
    vector<vector<LinkNode *>> qNodes;
    for (int i = 0; i < pTable_.size(); ++i) {
        vector<LinkNode *> nodes;
        pNodes.push_back(nodes);
    }
    for (int i = 0; i < qTable_.size(); ++i) {
        vector<LinkNode *> nodes;
        qNodes.push_back(nodes);
    }
    for (int i = 0; i < pTable_.size(); ++i) {
        for (int j = 0; j < qTable_.size(); ++j) {
            pair<int, int> p1 = p.vertex[pTable_[i]->tableId][pTable_[i]->vertexId];
            pair<int, int> p2 = p.vertex[pTable_[i]->next->tableId][pTable_[i]->next->vertexId];
            pair<int, int> q1 = q.vertex[qTable_[j]->tableId][qTable_[j]->vertexId];
            pair<int, int> q2 = q.vertex[qTable_[j]->next->tableId][qTable_[j]->next->vertexId];
            double r1 = 0, r2 = 0, x = 0, y = 0;
            bool cross = isCross(p1, p2, q1, q2, r1, r2, x, y);
            if (cross) {
                int type = OUT_NODE;
                if (crossProduct(p1, p2, q1, q2) > 0) {
                    type = IN_NODE;
                }
                LinkNode *newPnode = new LinkNode(x, y, r1, type);
                LinkNode *newQnode = new LinkNode(x, y, r2, type);
                newPnode->equal = newQnode;
                newQnode->equal = newPnode;
                pNodes[i].push_back(newPnode);
                qNodes[j].push_back(newQnode);
            }
        }
    }
    for (int i = 0; i < pNodes.size(); ++i) {
        sort(pNodes[i].begin(), pNodes[i].end(), compare);
    }
    for (int i = 0; i < qNodes.size(); ++i) {
        sort(qNodes[i].begin(), qNodes[i].end(), compare);
    }
    for (int i = 0; i < pTable_.size(); ++i) {
        pTable.push_back(pTable_[i]);
        LinkNode *last = pTable_[i], *next = pTable_[i]->next;
        for (int j = 0; j < pNodes[i].size(); ++j) {
            LinkNode *node = pNodes[i][j];
            pTable.push_back(node);
            last->next = node;
            last = node;
        }
        last->next = next;
    }
    for (int i = 0; i < qTable_.size(); ++i) {
        qTable.push_back(qTable_[i]);
        LinkNode *last = qTable_[i], *next = qTable_[i]->next;
        for (int j = 0; j < qNodes[i].size(); ++j) {
            LinkNode *node = qNodes[i][j];
            qTable.push_back(node);
            last->next = node;
            last = node;
        }
        last->next = next;
    }
}

double Utility::getArea(Polygon &p) {
    vector<pair<int, int>> nodes = p.vertex[0];
    int area = 0;
    for (int i = 0; i < nodes.size() - 1; ++i) {
        area += nodes[i].first * nodes[i + 1].second;
        area -= nodes[i + 1].first * nodes[i].second;
    }
    area += nodes[nodes.size() - 1].first * nodes[0].second;
    area -= nodes[0].first * nodes[nodes.size() - 1].second;
    return 0.5 * abs(area);
}

double Utility::angleBetween(double x1, double y1, double x2, double y2) {
    double sinValue = (x1 * y2 - x2 * y1) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2));
    double cosValue = (x1 * x2 + y1 * y2) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2));
    if (cosValue >= 1.0) {
        return 0;
    }
    if (cosValue <= -1.0) {
        return 3.1416;
    }
    double a = acos(cosValue);
    if (sinValue < 0) {
        a = -a;
    }
    return a;
}

bool Utility::isPointInPolygon(pair<int, int> &point, Polygon &p) {
    double i = point.first, j = point.second;
    bool isInside = false;
    double totalAngle = 0.0;
    for (int k = 0; k < p.vertex[0].size(); ++k) {
        double x1 = p.vertex[0][k].first - i;
        double y1 = p.vertex[0][k].second - j;
        double x2 = (k == p.vertex[0].size() - 1) ? (p.vertex[0][0].first - i) : (p.vertex[0][k + 1].first - i);
        double y2 = (k == p.vertex[0].size() - 1) ? (p.vertex[0][0].second - j) : (p.vertex[0][k + 1].second - j);
        double a = angleBetween(x1, y1, x2, y2);
        totalAngle += a;
    }
    if (abs(totalAngle) > 6) {
        isInside = true;
    }
    if (isInside) {
        for (int k = 1; k < p.vertex.size(); ++k) {
            totalAngle = 0.0;
            for (int l = 0; l < p.vertex[k].size(); ++l) {
                double x1 = p.vertex[k][l].first - i;
                double y1 = p.vertex[k][l].second - j;
                double x2 = (l == p.vertex[k].size() - 1) ? (p.vertex[k][0].first - i) : (p.vertex[k][l + 1].first - i);
                double y2 = (l == p.vertex[k].size() - 1) ? (p.vertex[k][0].second - j) : (p.vertex[k][l + 1].second - j);
                double a = angleBetween(x1, y1, x2, y2);
                totalAngle += a;
            }
            if (abs(totalAngle) > 6) {
                isInside = false;
                break;
            }
        }
    }
    return isInside;
}
