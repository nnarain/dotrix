#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QPainter>
#include <QRect>
#include <QRectF>
#include <QColor>

#include <functional>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	refresh_timer_(this),
	screen_(new Screen),
	updater_(new CoreUpdater(gameboycore_))
{
    ui->setupUi(this);

	// setup screen. MainWindow now owns screen_
	setCentralWidget(screen_);

	// update screen at ~16 ms (60 Hz)
	connect(&refresh_timer_, SIGNAL(timeout()), this, SLOT(updateScreen()));
	refresh_timer_.start(16);

	// core updater
	connect(updater_, SIGNAL(finished()), updater_, SLOT(deleteLater()));
}

void MainWindow::updateScreen()
{
	update();
}

void MainWindow::loadROM(const QString& filename)
{
    // read rom file
    QFile file(filename);
    std::vector<uint8_t> buffer;
    buffer.resize(file.size());

	file.open(QIODevice::ReadOnly);
    file.read((char*)&buffer[0], file.size());

    // load rom into the core
    gameboycore_.loadROM(&buffer[0], buffer.size());

    gameboycore_.getGPU()->setRenderCallback(
        std::bind(
            &Screen::gpuCallback,
            screen_,
            std::placeholders::_1, std::placeholders::_2
        )
    );

	updater_->start();

	ui->statusBar->showMessage("Playing " + filename);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	updater_->stop();
	updater_->wait();

	QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}
