#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QCoreApplication::addLibraryPath("./");//the DLL fix
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
