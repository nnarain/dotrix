#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>

#include <functional>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	refresh_timer_(this),
	screen_(new Screen),
	updater_(gameboycore_)
{
    ui->setupUi(this);

	// setup screen. MainWindow now owns screen_
	setCentralWidget(screen_);

	// update screen at ~16 ms (60 Hz)
	connect(&refresh_timer_, SIGNAL(timeout()), this, SLOT(updateScreen()));
	refresh_timer_.start(16);

	// initialize menu items
	initMenuActions();
}

void MainWindow::updateScreen()
{
	update();
}

void MainWindow::openFile()
{
	auto was_running = false;

	// if the core updater is running, stop it
	if (updater_.isRunning())
	{
		updater_.stop();
		was_running = true;
	}

	auto filename = QFileDialog::getOpenFileName(this, tr("Open ROM"), "", tr("ROM (*.gb)"));
	
	// check for content
	auto valid_rom = false;
	if (filename.size() > 1)
	{
		loadROM(filename);
		valid_rom = true;
	}

	if (was_running || valid_rom)
	{
		updater_.start();
	}
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

	input_.setJoypad(gameboycore_.getJoypad());

	ui->statusBar->showMessage("Loaded " + filename);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	updater_.stop();

	QMainWindow::closeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	input_.process(Input::State::PRESSED, event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
	input_.process(Input::State::RELEASED, event->key());
}

void MainWindow::initMenuActions()
{
	connect(ui->actionOpenRom, SIGNAL(triggered()), this, SLOT(openFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
