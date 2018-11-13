#include "polygon.h"

Polygon::Polygon() {

}

Polygon::Polygon(QRgb c) {
    color = c;
}

void Polygon::addOutPoints(vector<pair<int, int>> &p) {
    if (vertex.size() == 0) {
        vertex.push_back(p);
    } else {
        vertex[0] = p;
    }
}

void Polygon::addInPoints(vector<pair<int, int>> &p) {
    vertex.push_back(p);
}

void Polygon::move(int direction, int distance) {
    for (int i = 0; i < vertex.size(); ++i) {
        for (int j = 0; j < vertex[i].size(); ++j) {
            if (direction == 0) {
                vertex[i][j].first -= distance;
            } else if (direction == 1) {
                vertex[i][j].first += distance;
            } else if (direction == 2) {
                vertex[i][j].second -= distance;
            } else {
                vertex[i][j].second += distance;
            }
        }
    }
}

void Polygon::rotate(int angle) {
    double a = (double)angle * 3.1416 / 180.0;
    if (vertex.size() == 0) {
        return;
    }
    pair<int, int> center;
    int xTotal = 0, yTotal = 0;
    for (int i = 0; i < vertex[0].size(); ++i) {
        xTotal += vertex[0][i].first;
        yTotal += vertex[0][i].second;
    }
    center.first = (double)xTotal / (double)vertex[0].size();
    center.second = (double)yTotal / (double)vertex[0].size();
    for (int i = 0; i < vertex.size(); ++i) {
        for (int j = 0; j < vertex[i].size(); ++j) {
            double x = vertex[i][j].first - center.first;
            double y = vertex[i][j].second - center.second;
            double newX = x * cos(a) - y * sin(a);
            double newY = y * cos(a) + x * sin(a);
            vertex[i][j].first = center.first + newX;
            vertex[i][j].second = center.second + newY;
        }
    }
}

void Polygon::scale(double r) {
    if (r <= 0 || r >= 100.0) {
        return;
    }
    if (vertex.size() == 0) {
        return;
    }
    pair<int, int> center;
    int xTotal = 0, yTotal = 0;
    for (int i = 0; i < vertex[0].size(); ++i) {
        xTotal += vertex[0][i].first;
        yTotal += vertex[0][i].second;
    }
    center.first = (double)xTotal / (double)vertex[0].size();
    center.second = (double)yTotal / (double)vertex[0].size();
    for (int i = 0; i < vertex.size(); ++i) {
        for (int j = 0; j < vertex[i].size(); ++j) {
            double x = vertex[i][j].first - center.first;
            double y = vertex[i][j].second - center.second;
            double newX = x * r;
            double newY = y * r;
            vertex[i][j].first = center.first + newX;
            vertex[i][j].second = center.second + newY;
        }
    }
}

void Polygon::flipHorizontal() {
    if (vertex.size() == 0) {
        return;
    }
    int centerX;
    int xTotal = 0;
    for (int i = 0; i < vertex[0].size(); ++i) {
        xTotal += vertex[0][i].first;
    }
    centerX = (double)xTotal / (double)vertex[0].size();
    for (int i = 0; i < vertex.size(); ++i) {
        for (int j = 0; j < vertex[i].size(); ++j) {
            vertex[i][j].first = 2 * centerX - vertex[i][j].first;
        }
    }
}

void Polygon::flipVertical() {
    if (vertex.size() == 0) {
        return;
    }
    int centerY;
    int yTotal = 0;
    for (int i = 0; i < vertex[0].size(); ++i) {
        yTotal += vertex[0][i].second;
    }
    centerY = (double)yTotal / (double)vertex[0].size();
    for (int i = 0; i < vertex.size(); ++i) {
        for (int j = 0; j < vertex[i].size(); ++j) {
            vertex[i][j].second = 2 * centerY - vertex[i][j].second;
        }
    }
}
