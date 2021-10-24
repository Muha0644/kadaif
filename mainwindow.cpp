#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileSystemModel>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QSettings>
#include <QLayout>
#include <QSpacerItem>
#include "parser.h"
#include "locstuff.h"
#include "loceditor.h"


mainWindow::mainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::mainWindow){
	QSettings settings("muha0644","Kadaif");
	restoreGeometry(settings.value("main/geometry").toByteArray());
	restoreState(settings.value("main/windowState").toByteArray());
	ui->setupUi(this);

	QString path = settings.value("path").toString();
	if(path == ""){	// if path is not saved make the user chose one.
		ui->openFolder->trigger();
	} else {
		folderModel = new QFileSystemModel;
		folderModel->setRootPath(path);
		ui->fileListThing->setModel(folderModel);
		ui->fileListThing->setRootIndex(folderModel->index(path));
		ui->fileListThing->hideColumn(1);
		ui->fileListThing->hideColumn(2);
		ui->fileListThing->hideColumn(3);

		this->locAll = loadLoc();
	}
	ui->splitter->restoreState(settings.value("splitterSizes").toByteArray());
	QDir::setCurrent(path);
}

mainWindow::~mainWindow(){
	QSettings settings("muha0644","Kadaif");
	settings.setValue("main/geometry", saveGeometry());
	settings.setValue("main/windowState", saveState());
	delete folderModel;
	delete locAll;	//i mean the program is closed once you reach here, so this is redundant
	delete ui;
}

QWidget* mainWindow::setUpLocList(QString &path){			//holy fucking shit i should have used an object...
	QListWidget *locListWidget = new QListWidget;

	QWidget *holder = new QWidget;
	QVBoxLayout *vLayout = new QVBoxLayout(holder);		//do kids get deleted? I think they do.
	QHBoxLayout *buttonBar = new QHBoxLayout;
	QPushButton *saveButt = new QPushButton("Save");	//can also pass an icon as an argument.
	QPushButton *newButt = new QPushButton("New entry under selection");
	QPushButton *deleteButt = new QPushButton("Delete selection");
	QSpacerItem *spase = new QSpacerItem(10000,20,QSizePolicy::Preferred);

	buttonBar->addWidget(newButt);
	buttonBar->addWidget(deleteButt);
	buttonBar->addWidget(saveButt);
	buttonBar->addSpacerItem(spase);

	vLayout->addLayout(buttonBar);
	vLayout->addWidget(locListWidget);

	ui->splitter->addWidget(holder);
	loadLocFile(locListWidget, path);	//do not question why this is here
	return holder;
}

void mainWindow::on_fileListThing_doubleClicked(const QModelIndex &index){ //TODO: add enter trigger
	QSettings settings("muha0644","Kadaif");
	QString path = ((QFileSystemModel)ui->fileListThing->model()).filePath(index);
	path.remove(settings.value("path").toString());

	//ui->listThing->addItem(path + " | Type: " + (char)((int)parseType(path)));			//just debug st*ff

	if(parseType(path) == nothing){	//if nothing will be changed, do not do anything.
		return;
	}
	if(activeWidget){ delete activeWidget; activeWidget = nullptr;}	//something will be changed, delete the old
	switch(parseType(path)){
		case loc:{
			activeWidget = setUpLocList(path);
			//connect(temp,temp->itemDoubleClicked, ); //open loc editor when you click on an entry
			break;}
		case nothing:	//should never reach here
		default:
			return;
	}
	ui->splitter->restoreState(settings.value("splitterSizes").toByteArray());
}

void mainWindow::on_openFolder_triggered(){	//open mod folder in sidebar
	QString path = QFileDialog::getOpenFileName(this, "Chose mod folder location:", "", "Mod descriptor file (descriptor.mod)");
	if(path == "") return;
	path.chop(sizeof("descriptor.mod"));

	QSettings settings("muha0644", "Kadaif");
	settings.setValue("path", path);
	QDir::setCurrent(path);

	QPointer<QFileSystemModel> model = new QFileSystemModel;
	model->setRootPath(path);
	ui->fileListThing->setModel(model);
	ui->fileListThing->setRootIndex(model->index(path));
	ui->fileListThing->hideColumn(1);
	ui->fileListThing->hideColumn(2);
	ui->fileListThing->hideColumn(3);

	this->locAll = loadLoc();
}


void mainWindow::on_splitter_splitterMoved(int pos, int index){
	QSettings settings("muha0644", "Kadaif");
	settings.setValue("splitterSizes", ui->splitter->saveState());
}

void mainWindow::on_actionLocalization_editor_triggered(){
	locEditor *locw = new locEditor;
	locw->setAttribute(Qt::WA_DeleteOnClose);
	locw->show();
}

