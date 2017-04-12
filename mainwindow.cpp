#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_pushButton_clicked() {
    vector<vector<char> > arr;
    for (int i = 0; i < 5; ++i) arr.push_back(vector<char>(3));
    arr[1][1] = arr[1][2] = arr[2][1] = arr[3][1] = arr[3][2] = 1;
    World world(arr);

    world.expand(6);
    world.cellular(0x01e801d0);
    world.cellular(0x01f001e0);
    world.expand(2);
    world.cellular(0x01e801d0);
    world.cellular(0x01f001e0);
    world.rescale(1600, 800);
    world.cellular(0x01e801d0, 20);
    world.cellular(0x01f001e0);

    world.rescale(1600, 800);

    QPixmap pixmap(world.w, world.h);

    QPainter painter(&pixmap);
    QColor colors[2] {QColor(150, 200, 250), QColor(50, 150, 50)};
    arr = world.array();
    for (int x = 0; x < world.w; ++x)
        for (int y = 0; y < world.h; ++y)
            painter.fillRect(x, y, 1, 1, colors[arr[x][y]]);

    this->pixmap = pixmap;
    repaint();
}


void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
}
