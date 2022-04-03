#include <QApplication>
#include <QIcon>
#include <QtDebug>
#include "mainwindow.h"
#include <QTreeWidget>
#include <QSettings>
#include "dataclass.h"


int main(int argc, char *argv[]){
	qInstallMessageHandler(messHandler);
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/shared/taskbaricon.png"));


	mainWindow mainW;
	mainW.show();

	//QObject::connect(&mainW, &QMainWindow::destroyed, &app, &QApplication::quit);

	return app.exec();
}
