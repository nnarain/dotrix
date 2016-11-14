#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[])
{
	try
	{
		QApplication a(argc, argv);
		MainWindow w;
		w.show();

		return a.exec();
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return -1;
}
