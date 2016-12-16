#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

#include <functional>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	network_settings_(new NetworkSettings(this)),
	refresh_timer_(this),
	screen_(new Screen),
	input_(core_.getCore().getJoypad()),
	save_file_name_("")
{
    ui->setupUi(this);

	// setup screen
	connect(&core_, SIGNAL(scanline(gb::GPU::Scanline, int)), screen_, SLOT(update(gb::GPU::Scanline, int)));
	setCentralWidget(screen_);

	// update screen at ~16 ms (60 Hz)
	connect(&refresh_timer_, SIGNAL(timeout()), this, SLOT(updateScreen()));
	refresh_timer_.start(16);

	// network
	connect(network_settings_, SIGNAL(interfaceReady(QObject*)), this, SLOT(networkInterfaceReady(QObject*)));

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
	if (core_.isRunning())
	{
		core_.stop();
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
		core_.start();
	}
}

void MainWindow::loadROM(const QString& filename)
{
	// Save file info
	QFileInfo saveinfo(filename);
	save_file_name_ = QDir::cleanPath(saveinfo.canonicalPath() + QDir::separator() + saveinfo.baseName() + ".sav");

	core_.load(filename, save_file_name_);
	core_.setupCallbacks();

	ui->statusBar->showMessage("Loaded " + filename);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	core_.stop();

	core_.save(save_file_name_);

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

void MainWindow::networkInterfaceReady(QObject* obj)
{
	connect(&core_, SIGNAL(linkReady(uint8_t, gb::Link::Mode)), obj, SLOT(linkReady(uint8_t, gb::Link::Mode)));
	connect(obj, SIGNAL(recieved(uint8_t)), &core_, SLOT(linkRecieve(uint8_t)));
}

void MainWindow::initMenuActions()
{
	connect(ui->actionOpenRom, SIGNAL(triggered()), this, SLOT(openFile()));

	// dialogs
	connect(ui->actionNetwork, SIGNAL(triggered()), network_settings_, SLOT(show()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
