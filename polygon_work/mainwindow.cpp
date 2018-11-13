#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    img = new ImgWidget(this);
    ui->imgLayout->addWidget(img);

    polygonList = new QStringList();
    polygonListModel = new QStringListModel(this);
    ui->polygonList->setModel(polygonListModel);

    isChoosingCutPoly = false;
    selectedPolygonId = chosenPolygonId = -1;

    ui->colorR->setValidator(new QIntValidator(0, 255, this));
    ui->colorG->setValidator(new QIntValidator(0, 255, this));
    ui->colorB->setValidator(new QIntValidator(0, 255, this));

    ui->moveDis->setValidator(new QIntValidator(0, 100, this));

    ui->rotateAngle->setValidator(new QIntValidator(0, 360, this));

    ui->rotateAngle->setValidator(new QDoubleValidator(0.01, 99.99, 2, this));

    connect(ui->addPolygonBtn, SIGNAL(clicked(bool)), img, SLOT(setAddPolygonFlag()));
    connect(img, SIGNAL(addPolygonItem(QString)), this, SLOT(addListItem(QString)));
    connect(img, SIGNAL(removePolygonItem(QString)), this, SLOT(removeListItem(QString)));
    connect(ui->polygonList, SIGNAL(clicked(QModelIndex)), this, SLOT(polygonSelected(QModelIndex)));
    connect(ui->setColorBtn, SIGNAL(clicked(bool)), this, SLOT(setPolygonColor()));
    connect(ui->moveLeftBtn, SIGNAL(clicked(bool)), this, SLOT(movePolygonLeft()));
    connect(ui->moveRightBtn, SIGNAL(clicked(bool)), this, SLOT(movePolygonRight()));
    connect(ui->moveUpBtn, SIGNAL(clicked(bool)), this, SLOT(movePolygonUp()));
    connect(ui->moveDownBtn, SIGNAL(clicked(bool)), this, SLOT(movePolygonDown()));
    connect(ui->rotateLeftBtn, SIGNAL(clicked(bool)), this, SLOT(rotatePolygonLeft()));
    connect(ui->rotateRightBtn, SIGNAL(clicked(bool)), this, SLOT(rotatePolygonRight()));
    connect(ui->scaleBtn, SIGNAL(clicked(bool)), this, SLOT(scalePolygon()));
    connect(ui->horFlipBtn, SIGNAL(clicked(bool)), this, SLOT(flipPolygonHor()));
    connect(ui->verFlipBtn, SIGNAL(clicked(bool)), this, SLOT(flipPolygonVer()));
    connect(ui->addInCircleBtn, SIGNAL(clicked(bool)), this, SLOT(addPolygonInCircle()));
    connect(ui->showFillBtn, SIGNAL(clicked(bool)), this, SLOT(showPolygonFill()));
    connect(ui->removeBtn, SIGNAL(clicked(bool)), this, SLOT(removePolygon()));
    connect(ui->cutBtn, SIGNAL(clicked(bool)), this, SLOT(chooseCutPolygon()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addListItem(QString s) {
    polygonList->append(s);
    polygonListModel->setStringList(*polygonList);
}

void MainWindow::removeListItem(QString s) {
    int idToDelete = -1;
    for (int i = 0; i < polygonList->size(); ++i) {
        if (static_cast<QString>(polygonList->at(i)) == s) {
            idToDelete = i;
            break;
        }
    }
    if (idToDelete < 0) {
        return;
    }
    polygonList->removeAt(idToDelete);
    polygonListModel->setStringList(*polygonList);
}

void MainWindow::polygonSelected(QModelIndex id) {
    bool ok;
    if (isChoosingCutPoly) {
        isChoosingCutPoly = false;
        chosenPolygonId = id.data().toString().mid(8).toInt(&ok, 10) - 1;
        img->cutPolygon(selectedPolygonId, chosenPolygonId);
    } else {
        selectedPolygonId = id.data().toString().mid(8).toInt(&ok, 10) - 1;
    }
}

void MainWindow::setPolygonColor() {
    bool ok;
    int r = ui->colorR->text().toInt(&ok, 10);
    int g = ui->colorG->text().toInt(&ok, 10);
    int b = ui->colorB->text().toInt(&ok, 10);
    if (selectedPolygonId < 0) {
        return;
    }
    img->setPolygonColor(selectedPolygonId, qRgb(r, g, b));
}

void MainWindow::movePolygonLeft() {
    if (selectedPolygonId < 0) {
        return;
    }
    bool ok;
    img->movePolygon(selectedPolygonId, 0, ui->moveDis->text().toInt(&ok, 10));
}

void MainWindow::movePolygonRight() {
    if (selectedPolygonId < 0) {
        return;
    }
    bool ok;
    img->movePolygon(selectedPolygonId, 1, ui->moveDis->text().toInt(&ok, 10));
}

void MainWindow::movePolygonUp() {
    if (selectedPolygonId < 0) {
        return;
    }
    bool ok;
    img->movePolygon(selectedPolygonId, 2, ui->moveDis->text().toInt(&ok, 10));
}

void MainWindow::movePolygonDown() {
    if (selectedPolygonId < 0) {
        return;
    }
    bool ok;
    img->movePolygon(selectedPolygonId, 3, ui->moveDis->text().toInt(&ok, 10));
}

void MainWindow::rotatePolygonLeft() {
    if (selectedPolygonId < 0) {
        return;
    }
    bool ok;
    img->rotatePolygon(selectedPolygonId, -ui->rotateAngle->text().toInt(&ok, 10));
}

void MainWindow::rotatePolygonRight() {
    if (selectedPolygonId < 0) {
        return;
    }
    bool ok;
    img->rotatePolygon(selectedPolygonId, ui->rotateAngle->text().toInt(&ok, 10));
}

void MainWindow::scalePolygon() {
    if (selectedPolygonId < 0) {
        return;
    }
    bool ok;
    img->scalePolygon(selectedPolygonId, ui->scaleRate->text().toDouble(&ok));
}

void MainWindow::flipPolygonHor() {
    if (selectedPolygonId < 0) {
        return;
    }
    img->flipPolygonHor(selectedPolygonId);
}

void MainWindow::flipPolygonVer() {
    if (selectedPolygonId < 0) {
        return;
    }
    img->flipPolygonVer(selectedPolygonId);
}

void MainWindow::addPolygonInCircle() {
    if (selectedPolygonId < 0) {
        return;
    }
    img->addPolygonInCircle(selectedPolygonId);
}

void MainWindow::showPolygonFill() {
    if (selectedPolygonId < 0) {
        return;
    }
    img->showPolygonFill(selectedPolygonId);
}

void MainWindow::removePolygon() {
    if (selectedPolygonId < 0) {
        return;
    }
    int idToDelete = -1;
    bool ok;
    for (int i = 0; i < polygonList->size(); ++i) {
        if (static_cast<QString>(polygonList->at(i)).mid(8).toInt(&ok, 10) - 1 == selectedPolygonId) {
            idToDelete = i;
            break;
        }
    }
    if (idToDelete < 0) {
        return;
    }
    polygonList->removeAt(idToDelete);
    polygonListModel->setStringList(*polygonList);
    img->removePolygon(selectedPolygonId);
}

void MainWindow::chooseCutPolygon() {
    isChoosingCutPoly = true;
}
