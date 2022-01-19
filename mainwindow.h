#pragma once
#include <QMainWindow>
#include <QtWidgets>
#include <QLayout>
#include "loc/locstuff.h"
#include "dataclass.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE

class mainWindow : public QMainWindow{
	Q_OBJECT

	public:
	mainWindow(dataClass *dataClass,QWidget *parent = nullptr);
	~mainWindow();

	QString cPath;
	QString wPath;
	dataClass* liveDB;

	private slots:
	void on_openFolder_triggered();
	void on_splitter_splitterMoved(int pos, int index);
	void on_fileListThing_doubleClicked(const QModelIndex &index);
	void on_actionLocalization_editor_triggered();
	void openMainWidget(QString path);
	void on_fileListThing_clicked(const QModelIndex &index);
	void on_newButt_clicked();
	void on_renameButt_clicked();
	void on_rmButt_clicked();

	private:
	void deleteActiveWidget();

	void nonononoedit();
	void nonononodelete();
	void nononononew();
	void nonononodup();
	void nonononoemty();

	QFileSystemModel *folderModel;
	Ui::mainWindow *ui;
	//QWidget *activeWidget;
	QWidget *activeWidget = nullptr;
	QWidget *setUpLocList(QString &path);
};
