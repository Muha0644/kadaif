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
#include "loc/locstuff.h"
#include "loc/loceditor.h"


mainWindow::mainWindow(dataClass *dataClass,QWidget *parent): QMainWindow(parent), ui(new Ui::mainWindow){
	this->liveDB = dataClass;
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
		QDir::setCurrent(path);

		if(!liveDB->locAll){		//should be the first and only time it's initialised
			liveDB->locAll = loadLoc();
		}
	}
	ui->splitter->restoreState(settings.value("splitterSizes").toByteArray());

}

mainWindow::~mainWindow(){
	QSettings settings("muha0644","Kadaif");
	settings.setValue("main/geometry", saveGeometry());
	settings.setValue("main/windowState", saveState());
	delete folderModel;
	delete ui;
}

void mainWindow::on_actionLocalization_editor_triggered(){
	locEditor *locw = new locEditor;
	locw->setAttribute(Qt::WA_DeleteOnClose);
	locw->setAttribute( Qt::WA_QuitOnClose, false );
	locw->show();
}

void mainWindow::nonononoedit(){			//SHOULD HAVE USED AN OBJECT
	QListWidget *real = static_cast<QListWidget*>(activeWidget->children()[4]);
	int cRow = real->currentRow();
	if(cRow < 0) return;
	locEditor *locw = new locEditor(liveDB,real->item(cRow));
	locw->setAttribute(Qt::WA_DeleteOnClose);
	locw->setAttribute( Qt::WA_QuitOnClose, false );
	locw->show();
	locw->entry.file = cPath.mid(sizeof("localization/"));
	connect(locw, &locEditor::saved, this, &mainWindow::openMainWidget);
}

void mainWindow::nonononodelete(){			//THIS SHOULD NOT BE IN `mainWindow`
	QListWidget *real = static_cast<QListWidget*>(activeWidget->children()[4]);
	QList<QString> *fileLocList = parseLocFile(cPath);

	int cRow = real->currentRow();

	if(cRow != -1){//bruh segfault
		fileLocList->remove(cRow);
		real->clear();
		QString entry;
		foreach(entry, *fileLocList){
			real->addItem(entry);
		}
	}
	real->setCurrentRow(cRow);
	saveLocFile(fileLocList,cPath);
	delete fileLocList;
}

void mainWindow::nononononew(){				//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	QListWidget *real = static_cast<QListWidget*>(activeWidget->children()[4]);
	QList<QString> *fileLocList = parseLocFile(cPath);

	fileLocList->insert(real->currentRow()+1, " ");
	real->insertItem(real->currentRow()+1, " ");

	saveLocFile(fileLocList,cPath);
	delete fileLocList;
	real->setCurrentRow(real->currentRow()+1);
	nonononoedit();
}

QWidget* mainWindow::setUpLocList(QString &path){			//holy fucking shit i should have used an object...
	QListWidget *locListWidget = new QListWidget;
	QWidget *holder = new QWidget;
	QVBoxLayout *vLayout = new QVBoxLayout(holder);		//do kids get deleted? I think they do.
	QHBoxLayout *buttonBar = new QHBoxLayout;
	QPushButton *editButt = new QPushButton("Edit");	//can also pass an icon as an argument.
	QPushButton *newButt = new QPushButton("New entry under selection");
	QPushButton *deleteButt = new QPushButton("Delete selection");
	QSpacerItem *spase = new QSpacerItem(10000,20,QSizePolicy::Preferred);

	//implement functions for the buttons. I don't want to convert everything into an object, even though it would be better
	connect(newButt, &QPushButton::clicked, this, &mainWindow::nononononew);
	connect(deleteButt, &QPushButton::clicked, this, &mainWindow::nonononodelete);
	connect(editButt, &QPushButton::clicked, this, &mainWindow::nonononoedit);
	connect(locListWidget, &QListWidget::itemDoubleClicked, this, &mainWindow::nonononoedit);

	buttonBar->addWidget(newButt);
	buttonBar->addWidget(deleteButt);
	buttonBar->addWidget(editButt);
	buttonBar->addSpacerItem(spase);

	vLayout->addLayout(buttonBar);
	vLayout->addWidget(locListWidget);

	ui->splitter->addWidget(holder);
	loadLocFile(locListWidget, path);	//do not question why this is here
	return holder;
}

void mainWindow::on_fileListThing_doubleClicked(const QModelIndex &index){	//TODO: add enter trigger
	QSettings settings("muha0644","Kadaif");								//^won't do
	cPath = ((QFileSystemModel)ui->fileListThing->model()).filePath(index);
	cPath.remove(settings.value("path").toString());
	openMainWidget(cPath);
}

void mainWindow::openMainWidget(QString path){
	if(parseType(path) == nothing){	//if nothing will be changed, do not do anything.
		return;
	}
	if(activeWidget){ delete activeWidget; activeWidget = nullptr;}	//something will be changed, delete the old
	switch(parseType(path)){
		case loc:{
			activeWidget = setUpLocList(path);
			break;}
		case nothing:	//should never reach here
		default:
			return;
	}
	QSettings settings("muha0644","Kadaif");
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

	if(!liveDB->locAll){		//should be the first and only time it's initialised
		liveDB->locAll = loadLoc();
	}
}


void mainWindow::on_splitter_splitterMoved(int pos, int index){
	QSettings settings("muha0644", "Kadaif");
	settings.setValue("splitterSizes", ui->splitter->saveState());
}


