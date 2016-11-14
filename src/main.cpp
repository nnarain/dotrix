#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <stdexcept>

#include "version.h"

int main(int argc, char *argv[])
{
	std::string title = "Dotrix " + version::get();

	try
	{
		QApplication a(argc, argv);
		MainWindow w;
		w.setWindowTitle(QString::fromStdString(title));
		w.show();

		return a.exec();
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return -1;
}
