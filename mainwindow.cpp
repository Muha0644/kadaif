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
#include "gfx/gfxstuff.h"


mainWindow::mainWindow(dataClass *dataClass,QWidget *parent): QMainWindow(parent), ui(new Ui::mainWindow){
	this->liveDB = dataClass;
	QSettings settings("muha0644","Kadaif");
	restoreGeometry(settings.value("main/geometry").toByteArray());
	restoreState(settings.value("main/windowState").toByteArray());
	ui->setupUi(this);

	QString path = settings.value("absPath").toString();
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
			liveDB->gfxAll = loadGfx();
		}
	}
	ui->splitter->restoreState(settings.value("splitterSizes").toByteArray());

#ifdef COOLBUTTONBAR
	ui->extraButt->setFrameShape(QFrame::Panel);
	ui->extraButt->setFrameShadow(QFrame::Raised); //optimised for dark theme on linux
#endif
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
	QListWidget *real = static_cast<QListWidget*>(activeWidget->children()[5]);	//!!! this will bug out if NOTITLE is defined
	int cRow = real->currentRow();
	if(cRow < 0) return;
	locEditor *locw = new locEditor(liveDB,real->item(cRow));
	locw->setAttribute(Qt::WA_DeleteOnClose);
	locw->setAttribute( Qt::WA_QuitOnClose, false );
	locw->show();
	locw->entry.file = cPath.sliced(sizeof("localization"));
	connect(locw, &locEditor::saved, this, &mainWindow::openMainWidget);
}

void mainWindow::nonononodelete(){			//THIS SHOULD NOT BE IN `mainWindow`
	QListWidget *real = static_cast<QListWidget*>(activeWidget->children()[5]);
	QList<QString> *fileLocList = parseLocFile(cPath);

	int cRow = real->currentRow();

	if(cRow != -1){//bruh segfault
		fileLocList->remove(cRow);
		real->clear();
		QString entry;
		foreach(entry, *fileLocList){
			real->addItem(entry);
		}
	} else {
		qDebug() << "!!!!!!!!!!!!!!!!!!!cRow is -1!!!!!!!!!!!!!!!!!!!!!!";	//remove if never reached
	}
	real->setCurrentRow(cRow);
	saveLocFile(fileLocList, cPath);
	delete fileLocList;
}

void mainWindow::nononononew(){				//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	QListWidget *real = static_cast<QListWidget*>(activeWidget->children()[5]);
	QList<QString> *fileLocList = parseLocFile(cPath);

	fileLocList->insert(real->currentRow()+1, " ");
	real->insertItem(real->currentRow()+1, " ");

	saveLocFile(fileLocList, cPath);
	delete fileLocList;
	real->setCurrentRow(real->currentRow()+1);
	nonononoedit();
}

void mainWindow::nonononodup(){
	QSettings settings("muha0644", "Kadaif");
	QMessageBox msgBox;
	msgBox.setText("Duplicate localization entries:");
	msgBox.setInformativeText(settings.value("locDup").toString());
	msgBox.exec();
}

void mainWindow::nonononoemty(){
	QString empty;
	///QSettings settings("muha0644", "Kadaif");
	foreach(auto entry, this->liveDB->locAll->values()){
		if(entry.value.replace("\"", "").trimmed() == ""){
			empty.append("Empty value found: " + entry.key + "\n" + entry.file + " on line: " + QString::number(entry.line) + "\n\n");
		};
	}
	QMessageBox msgBox;
	msgBox.setText("Empty localization keys:");
	msgBox.setInformativeText(empty);
	msgBox.exec();
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
#ifndef NOTITLE
	vLayout->setContentsMargins(5, 2, 0, 0);
	QLabel *title = new QLabel(path);
	QFont font;
	font.setBold(true);
	font.setPointSize(12);
	title->setFont(font);
	vLayout->addWidget(title);
#endif

	//I don't want to convert everything into an object, even though it would be better
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
	//amazingly documented code, i know. No need to thank me
	return holder;
}

void mainWindow::on_fileListThing_clicked(const QModelIndex &index){
	QSettings settings("muha0644","Kadaif");
	cPath = ((QFileSystemModel)ui->fileListThing->model()).filePath(index);
	cPath.remove(settings.value("absPath").toString()).remove(0,1);
}

void mainWindow::on_fileListThing_doubleClicked(const QModelIndex &index){
	openMainWidget(cPath);
	wPath = cPath;
}

//!!!! on_selection_changed will cause issues. navigating with a keyboard does not work

void mainWindow::deleteActiveWidget(){
	delete activeWidget;
	activeWidget = nullptr;
	for(auto child: ui->extraButt->children()){
		delete child;
	};
	ui->extraButt->setBaseSize(0,0);		//ignoring this for now, will probbably be problematic later...
	ui->extraButt->resize(0,0);
	ui->fileListThing->updateGeometry();
	return;
}

void mainWindow::openMainWidget(QString path){
	if(parseType(path) == nothing){	//if nothing will be changed, do not do anything.
		return;
	}
	if(activeWidget){ //something will be changed, delete the old
		deleteActiveWidget();
	}

	switch(parseType(path)){
		case loc:{
			activeWidget = setUpLocList(path);
			QGridLayout *container = new QGridLayout(ui->extraButt);
			QPushButton *checkDup = new QPushButton("Show duplicates");
			QPushButton *findEmty = new QPushButton("Find empty keys");
			connect(findEmty, &QPushButton::clicked, this, &mainWindow::nonononoemty);
			connect(checkDup, &QPushButton::clicked, this, &mainWindow::nonononodup);
			container->addWidget(checkDup,0,0);
			container->addWidget(findEmty,0,1);
			break;}
		case gfx:{

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
	settings.setValue("absPath", path);
	QDir::setCurrent(path);

	if(this->folderModel) delete this->folderModel;
	folderModel = new QFileSystemModel;
	folderModel->setRootPath(path);
	ui->fileListThing->setModel(folderModel);
	ui->fileListThing->setRootIndex(folderModel->index(path));
	ui->fileListThing->hideColumn(1);
	ui->fileListThing->hideColumn(2);
	ui->fileListThing->hideColumn(3);

	if(!liveDB->locAll){		//should be the first and only time it's initialised
		liveDB->locAll = loadLoc();
		liveDB->gfxAll = loadGfx();
	}
}


void mainWindow::on_splitter_splitterMoved(int pos, int index){
	QSettings settings("muha0644", "Kadaif");
	settings.setValue("splitterSizes", ui->splitter->saveState());
}


void mainWindow::on_newButt_clicked(){
	QDir root;
	QFileInfo finfo(cPath);
	QString newpath;	//relative path, folder always ends with a /
	if(finfo.isFile()){
			newpath = root.filePath(cPath);
			newpath.remove(finfo.fileName());
	} else newpath = cPath+"/";
	if(newpath == "/") newpath = "./";

	bool ok;
	QString text = QInputDialog::getText(this,"" , "Choose a name for the file.\n If you want to make a new folder, put a \"/\" after the name.",
										 QLineEdit::Normal,"" , &ok);
	if(!ok || text.isEmpty()) return;
	if(root.exists(newpath+text)){
		qCritical() << "File '" + newpath+text + "' already exists.";
		return;
	}

	if(text.endsWith("/")){	//directory
		if(!root.mkpath(newpath+text)) qCritical() << "Could not create folder '" + newpath+text + "'. Does it already exist?";
		return;
	}

	QFile shittyImplementation(newpath+text);//file
	if(shittyImplementation.exists()){ //this seems to do nothing, but i am too scared to remove it and too lazy to test it
		qCritical() << "File '" + newpath+text + "' already exists bro.";
		return;
	}
	if(!shittyImplementation.open(QIODevice::WriteOnly | QIODevice::Text)){
		qCritical() << "Could not create file '" + newpath+text + "'";
		return;
	}
	shittyImplementation.close();
}

void mainWindow::on_renameButt_clicked(){
	if(cPath==""){
		qCritical() << "Bruh, how about you select something first?";
		return;
	}
	QDir root;
	QFileInfo finfo(cPath);
	QString newpath;
	newpath = root.filePath(cPath);
	newpath.remove(finfo.fileName());

	if(newpath == "/") newpath = "./";

	bool ok;
	QString text = QInputDialog::getText(this,"" , "New name:", QLineEdit::Normal, finfo.fileName(), &ok);
	if(!ok || text.isEmpty()) return;
	if(root.exists(newpath+text)){
		qCritical() << "File '" + newpath+text + "' already exists.";
		return;
	}

	if(finfo.isFile()){
		QFile::rename(cPath, newpath+text);
		return;
	} else {
		root.rename(cPath, newpath+text);
	}
}

void mainWindow::on_rmButt_clicked(){
	//maybe ask for a confirmation?
	if(cPath == wPath){
		wPath = "";
		deleteActiveWidget();
	}
	if(cPath==""){
		qCritical() << "Bruh, how about you select something first?";
		return;
	}
	QModelIndex index = this->folderModel->index(cPath);
	if(!this->folderModel->remove(index)){
		//deleted the file, if flase that means it's likely a folder
		if(!this->folderModel->rmdir(index)){
			qCritical() << "Could not delete " + cPath + ". Something is seriously wrong. You should panic.";
		}
	}
	cPath = "";
}

