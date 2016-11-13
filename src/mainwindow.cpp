#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QPainter>
#include <QRect>

#include <functional>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    canvas_(160, 144, QImage::Format_ARGB32)
{
    ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawRect(QRect(50, 50, 160, 144));
}

void MainWindow::gpuCallback(gb::GPU::Scanline scanline, int line)
{

}

void MainWindow::loadROM(const QString& filename)
{
    // read rom file
    QFile file(filename);
    std::vector<uint8_t> buffer;
    buffer.resize(file.size());

    file.read((char*)&buffer[0], file.size());

    // load rom into the core
    gameboycore_.loadROM(&buffer[0], buffer.size());

    gameboycore_.getGPU()->setRenderCallback(
        std::bind(
            &MainWindow::gpuCallback,
            this,
            std::placeholders::_1, std::placeholders::_2
        )
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
