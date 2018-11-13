#ifndef POLYGON_H
#define POLYGON_H

#include <QRgb>

#include <vector>

using namespace std;

class Polygon {
public:
    vector<vector<pair<int, int>>> vertex;
    QRgb color;

public:
    Polygon();
    Polygon(QRgb c);
    void addOutPoints(vector<pair<int, int>> &p);
    void addInPoints(vector<pair<int, int>> &p);
    void move(int direction, int distance);
    void rotate(int angle);
    void scale(double r);
    void flipHorizontal();
    void flipVertical();
};

#endif // POLYGON_H
