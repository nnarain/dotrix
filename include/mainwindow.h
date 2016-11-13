#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QImage>

#include <gameboycore/gameboycore.h>

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

protected:
    void MainWindow::paintEvent(QPaintEvent* event);

private:
    void gpuCallback(gb::GPU::Scanline scanline, int line);

private:
    Ui::MainWindow *ui;

    QImage canvas_;
    gb::GameboyCore gameboycore_;
};

#endif // MAINWINDOW_H
