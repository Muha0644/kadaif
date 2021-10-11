#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileSystemModel>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QPointer>
#include <QSettings>
#include "parser.h"

mainWindow::mainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::mainWindow){
	QSettings *settings = new QSettings("muha0644","Kadaif");
	ui->setupUi(this);

	QString path = settings->value("path").toString();
	if(path == ""){	// if path is not saved make the user chose one.
		ui->openFolder->trigger();
	} else {
		QPointer<QFileSystemModel> model = new QFileSystemModel;
		model->setRootPath(path);
		ui->fileListThing->setModel(model);
		ui->fileListThing->setRootIndex(model->index(path));
		ui->fileListThing->hideColumn(1);
		ui->fileListThing->hideColumn(2);
		ui->fileListThing->hideColumn(3);
	}
	ui->splitter->restoreState(settings->value("splitterSizes").toByteArray());

	delete settings;
}

mainWindow::~mainWindow(){
	delete ui;
}

void mainWindow::on_openFolder_triggered(){	//open mod folder in sidebar
	QString path = QFileDialog::getOpenFileName(this, "Chose mod folder location:", "", "Mod descriptor file (descriptor.mod)");
	if(path == "") return;
	path.chop(sizeof("descriptor.mod"));

	QSettings *settings = new QSettings("muha0644", "Kadaif");
	settings->setValue("path", path);
	QDir::setCurrent(path);

	QPointer<QFileSystemModel> model = new QFileSystemModel;
	model->setRootPath(path);
	ui->fileListThing->setModel(model);
	ui->fileListThing->setRootIndex(model->index(path));
	ui->fileListThing->hideColumn(1);
	ui->fileListThing->hideColumn(2);
	ui->fileListThing->hideColumn(3);

	delete settings;
}


void mainWindow::on_splitter_splitterMoved(int pos, int index){
	QSettings *settings = new QSettings("muha0644", "Kadaif");
	settings->setValue("splitterSizes", ui->splitter->saveState());
	delete settings;
}


void mainWindow::on_fileListThing_doubleClicked(const QModelIndex &index){
	QSettings *settings = new QSettings("muha0644","Kadaif");
	QString path = ((QFileSystemModel)ui->fileListThing->model()).filePath(index);
	path.remove(settings->value("path").toString());
	delete settings;

	ui->listThing->addItem(path + " | Type: " + (char)((int)parser(path)));
}

