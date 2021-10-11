#include <QApplication>
#include <QIcon>
#include <QtDebug>
#include "mainwindow.h"
#include <QTreeWidget>
#include <QSettings>
#include "parser.h"


int main(int argc, char *argv[]){
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/shared/taskbaricon.png"));

	mainWindow mainW;
	mainW.show();

	return app.exec();
}
