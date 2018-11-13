#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include "imgwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ImgWidget *img;
    QStringList *polygonList;
    QStringListModel *polygonListModel;
    int selectedPolygonId;
    int chosenPolygonId;
    bool isChoosingCutPoly;

public slots:
    void addListItem(QString s);
    void removeListItem(QString s);
    void polygonSelected(QModelIndex id);
    void setPolygonColor();
    void movePolygonLeft();
    void movePolygonRight();
    void movePolygonUp();
    void movePolygonDown();
    void rotatePolygonLeft();
    void rotatePolygonRight();
    void scalePolygon();
    void flipPolygonHor();
    void flipPolygonVer();
    void addPolygonInCircle();
    void showPolygonFill();
    void removePolygon();
    void chooseCutPolygon();
};

#endif // MAINWINDOW_H
