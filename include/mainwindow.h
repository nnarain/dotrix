#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QTimer>

#include "screen.h"
#include "core_updater.h"

#include <gameboycore/gameboycore.h>

#include <thread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void MainWindow::loadROM(const QString& filename);

private slots:
	void updateScreen();

private:
	void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;

	QTimer refresh_timer_;
	Screen* screen_;
	CoreUpdater* updater_;

    gb::GameboyCore gameboycore_;
};


#endif // MAINWINDOW_H
