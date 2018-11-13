#ifndef IMGWIDGET_H
#define IMGWIDGET_H

#include <QWidget>
#include <QImage>
#include <QRgb>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#include "polygon.h"
#include "utility.h"

namespace Ui {
class ImgWidget;
}

class ImgWidget : public QWidget
{
    Q_OBJECT

    const static int width = 600;
    const static int height = 600;
    const static int w = 150;
    const static int h = 150;
    const static int d = 4;

public:
    explicit ImgWidget(QWidget *parent = 0);
    ~ImgWidget();

public:
    void initImg();
    void drawPoint(int x, int y, QRgb c);
    void drawLine(int x1, int y1, int x2, int y2, QRgb c);
    void drawPolygon(Polygon &p);
    void drawPolygonFill(Polygon &p);
    void drawScene(QRgb map[w][h]);
    void drawAll();

    // Polygon Operations
    void setPolygonColor(int id, QRgb c);
    void movePolygon(int id, int direction, int distance);
    void rotatePolygon(int id, int angle);
    void scalePolygon(int id, double r);
    void flipPolygonHor(int id);
    void flipPolygonVer(int id);
    void addPolygonInCircle(int id);
    void showPolygonFill(int id);
    void removePolygon(int id);
    void cutPolygon(int id1, int id2);

private:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    void setMap(int x, int y, QRgb c);
    pair<int, int> screen2scene(pair<int, int> pos);
    double angleBetween(double x1, double y1, double x2, double y2);
    bool contains(vector<int> &v, int val);

private:
    Ui::ImgWidget *ui;
    QImage *img;
    QRgb pixelMap[w][h];
    vector<Polygon> polygons;

    // Add Polygon
    bool isAddingPolygon;
    pair<int, int> lastPos;
    QRgb lastMap[w][h];
    Polygon *newPolygon;
    vector<pair<int, int>> points;

    // Add InCircle
    bool isAddingCircle;
    int pCircleId;

    // Remove Polygon
    vector<int> removedIds;

public slots:
    void setAddPolygonFlag();

signals:
    void addPolygonItem(QString);
    void removePolygonItem(QString);
};

#endif // IMGWIDGET_H
