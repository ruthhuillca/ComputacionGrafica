#include "imgwidget.h"
#include "ui_imgwidget.h"

ImgWidget::ImgWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ImgWidget) {
    ui->setupUi(this);

    img = new QImage(width, height, QImage::Format_RGB32);

    initImg();

    isAddingPolygon = false;
    isAddingCircle = false;
    lastPos = make_pair(-1, -1);
}

ImgWidget::~ImgWidget() {
    delete ui;
}

void ImgWidget::paintEvent(QPaintEvent *) {
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            for (int k = 0; k < d; ++k) {
                for (int l = 0; l < d; ++l) {
                    img->setPixel(i * d + k, j * d + l, pixelMap[i][j]);
                }
            }
        }
    }
    QPainter p(this);
    p.drawImage(QRect(0, 0, width, height), *img);
}

void ImgWidget::mouseReleaseEvent(QMouseEvent *e) {
    if (isAddingPolygon) {
        pair<int, int> pos = screen2scene(make_pair(e->x(), e->y()));
        if (points.size() > 0 && pos.first == points[0].first && pos.second == points[0].second) {
            newPolygon->addOutPoints(points);
            polygons.push_back(*newPolygon);
            setCursor(Qt::ArrowCursor);
            emit addPolygonItem(QString::fromStdString("Polygon_") + QString::number(polygons.size()));
            isAddingPolygon = false;
        } else {
            points.push_back(pos);
            if (lastPos.first != -1) {
                drawLine(lastPos.first, lastPos.second, pos.first, pos.second, qRgb(0, 0, 0));
            } else {
                drawPoint(pos.first, pos.second, qRgb(0, 0, 0));
            }
            lastPos = pos;
            for (int i = 0; i < w; ++i) {
                for (int j = 0; j < h; ++j) {
                    lastMap[i][j] = pixelMap[i][j];
                }
            }
        }
    } else if (isAddingCircle) {
        pair<int, int> pos = screen2scene(make_pair(e->x(), e->y()));
        if (points.size() > 0 && pos.first == points[0].first && pos.second == points[0].second) {
            polygons[pCircleId].addInPoints(points);
            setCursor(Qt::ArrowCursor);
            isAddingCircle = false;
        } else {
            points.push_back(pos);
            if (lastPos.first != -1) {
                drawLine(lastPos.first, lastPos.second, pos.first, pos.second, qRgb(0, 0, 0));
            } else {
                drawPoint(pos.first, pos.second, qRgb(0, 0, 0));
            }
            lastPos = pos;
            for (int i = 0; i < w; ++i) {
                for (int j = 0; j < h; ++j) {
                    lastMap[i][j] = pixelMap[i][j];
                }
            }
        }
    }
}

void ImgWidget::mouseMoveEvent(QMouseEvent *e) {
    if (isAddingPolygon) {
        drawScene(lastMap);
        if (lastPos.first != -1) {
            pair<int, int> pos = screen2scene(make_pair(e->x(), e->y()));
            if (points.size() > 0 && pos.first == points[0].first && pos.second == points[0].second) {
                setCursor(Qt::PointingHandCursor);
            } else {
                setCursor(Qt::ArrowCursor);
            }
            drawLine(lastPos.first, lastPos.second, pos.first, pos.second, qRgb(0, 0, 0));
        }
    } else if (isAddingCircle) {
        drawScene(lastMap);
        if (lastPos.first != -1) {
            pair<int, int> pos = screen2scene(make_pair(e->x(), e->y()));
            if (points.size() > 0 && pos.first == points[0].first && pos.second == points[0].second) {
                setCursor(Qt::PointingHandCursor);
            } else {
                setCursor(Qt::ArrowCursor);
            }
            drawLine(lastPos.first, lastPos.second, pos.first, pos.second, qRgb(0, 0, 0));
        }
    }
}

void ImgWidget::initImg() {
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            setMap(i, j, qRgb(255, 255, 255));
        }
    }
    update();
}

void ImgWidget::drawPoint(int x, int y, QRgb c) {
    setMap(x, y, c);
    update();
}

void ImgWidget::drawLine(int x1, int y1, int x2, int y2, QRgb c) {
    int x, y, dx, dy, sx, sy, se;
    double k, e;
    x = x1;
    y = y1;
    dx = x2 - x1;
    dy = y2 - y1;
    sx = dx > 0 ? 1 : -1;
    sy = dy > 0 ? 1 : -1;
    k = (double)dy / (double)dx;
    e = k > 0 ? -0.5 : 0.5;
    se = k > 0 ? -1 : 1;
    if (dx == 0) {
        for (int i = 0; i <= abs(dy); ++i) {
            setMap(x, y, c);
            y = y + sy;
        }
    } else if (dy == 0) {
        for (int i = 0; i <= abs(dx); ++i) {
            setMap(x, y, c);
            x = x + sx;
        }
    } else if (abs(k) <= 1) {
        for (int i = 0; i <= abs(dx); ++i) {
            setMap(x, y, c);
            x = x + sx;
            e = e + k;
            if ((k > 0 && e >= 0) || (k < 0 && e <= 0)) {
                y = y + sy;
                e = e + se;
            }
        }
    } else {
        k = 1.0 / k;
        for (int j = 0; j <= abs(dy); ++j) {
            setMap(x, y, c);
            y = y + sy;
            e = e + k;
            if ((k > 0 && e >= 0) || (k < 0 && e <= 0)) {
                x = x + sx;
                e = e + se;
            }
        }
    }
    update();
}

void ImgWidget::drawPolygon(Polygon &p) {
    for (int i = 0; i < p.vertex.size(); ++i) {
        for (int j = 0; j < p.vertex[i].size() - 1; ++j) {
            drawLine(p.vertex[i][j].first, p.vertex[i][j].second, p.vertex[i][j + 1].first, p.vertex[i][j + 1].second, p.color);
        }
        drawLine(p.vertex[i][p.vertex[i].size() - 1].first, p.vertex[i][p.vertex[i].size() - 1].second, p.vertex[i][0].first, p.vertex[i][0].second, p.color);
    }
    update();
}

void ImgWidget::drawPolygonFill(Polygon &p) {
    drawPolygon(p);
    if (p.vertex.size() == 0) {
        return;
    }
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
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
            if (isInside) {
                setMap(i, j, p.color);
            }
        }
    }
}

void ImgWidget::drawScene(QRgb map[w][h]) {
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            setMap(i, j, map[i][j]);
        }
    }
    update();
}

void ImgWidget::drawAll() {
    initImg();
    for (int i = 0; i < polygons.size(); ++i) {
        if (!contains(removedIds, i)) {
            drawPolygon(polygons[i]);
        }
    }
    update();
}

void ImgWidget::setPolygonColor(int id, QRgb c) {
    if (id >= polygons.size() || id < 0) {
        return;
    }
    polygons[id].color = c;
    drawAll();
}

void ImgWidget::movePolygon(int id, int direction, int distance) {
    // 0 - left    1 - right    2 - up    3 - down
    if (id >= polygons.size() || id < 0) {
        return;
    }
    polygons[id].move(direction, distance);
    drawAll();
}

void ImgWidget::rotatePolygon(int id, int angle) {
    // Default rotate clockwise
    if (id >= polygons.size() || id < 0) {
        return;
    }
    polygons[id].rotate(angle);
    drawAll();
}

void ImgWidget::scalePolygon(int id, double r) {
    if (id >= polygons.size() || id < 0) {
        return;
    }
    polygons[id].scale(r);
    drawAll();
}

void ImgWidget::flipPolygonHor(int id) {
    if (id >= polygons.size() || id < 0) {
        return;
    }
    polygons[id].flipHorizontal();
    drawAll();
}

void ImgWidget::flipPolygonVer(int id) {
    if (id >= polygons.size() || id < 0) {
        return;
    }
    polygons[id].flipVertical();
    drawAll();
}

void ImgWidget::addPolygonInCircle(int id) {
    pCircleId = id;
    points.clear();
    lastPos.first = lastPos.second = -1;
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            lastMap[i][j] = pixelMap[i][j];
        }
    }
    isAddingCircle = true;
}

void ImgWidget::showPolygonFill(int id) {
    if (id >= polygons.size() || id < 0) {
        return;
    }
    drawAll();
    drawPolygonFill(polygons[id]);
}

void ImgWidget::removePolygon(int id) {
    if (id >= polygons.size() || id < 0) {
        return;
    }
    removedIds.push_back(id);
    drawAll();
}

void ImgWidget::cutPolygon(int id1, int id2) {
    Utility u;
    vector<Polygon> cutResults = u.WeilerAtherton(polygons[id1], polygons[id2]);
    removePolygon(id1);
    removePolygon(id2);
    emit removePolygonItem(QString::fromStdString("Polygon_") + QString::number(id1 + 1));
    emit removePolygonItem(QString::fromStdString("Polygon_") + QString::number(id2 + 1));
    for (int i = 0; i < cutResults.size(); ++i) {
        cutResults[i].color = qRgb(0, 0, 0);
        polygons.push_back(cutResults[i]);
        emit addPolygonItem(QString::fromStdString("Polygon_") + QString::number(polygons.size()));
    }
    drawAll();
}

void ImgWidget::setAddPolygonFlag() {
    newPolygon = new Polygon(qRgb(0, 0, 0));
    points.clear();
    lastPos.first = lastPos.second = -1;
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            lastMap[i][j] = pixelMap[i][j];
        }
    }
    isAddingPolygon = true;
}

void ImgWidget::setMap(int x, int y, QRgb c) {
    if (x < 0 || y < 0 || x >= w || y >= h) {
        return;
    }
    pixelMap[x][y] = c;
}

pair<int, int> ImgWidget::screen2scene(pair<int, int> pos) {
    return make_pair(pos.first / d, pos.second / d);
}

double ImgWidget::angleBetween(double x1, double y1, double x2, double y2) {
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

bool ImgWidget::contains(vector<int> &v, int val) {
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] == val) {
            return true;
        }
    }
    return false;
}
