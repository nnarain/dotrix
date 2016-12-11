#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QTimer>
#include <QKeyEvent>

#include "qgameboycore.h"
#include "screen.h"
#include "core_updater.h"
#include "input.h"

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

	/* Menu slots */

	void openFile();

private:
	/* Window Events */

	void closeEvent(QCloseEvent *event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

	void initMenuActions();

private:
    Ui::MainWindow *ui;

//	gb::GameboyCore gameboycore_;

	QGameboyCore core_;

	QTimer refresh_timer_;
	Screen* screen_;
	Input input_;

	QString save_file_name_;
};


#endif // MAINWINDOW_H
