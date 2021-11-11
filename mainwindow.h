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
	dataClass* liveDB;

	void nonononoedit();
	void nonononodelete();
	void nononononew();

	private slots:
	void on_openFolder_triggered();
	void on_splitter_splitterMoved(int pos, int index);
	void on_fileListThing_doubleClicked(const QModelIndex &index);
	void on_actionLocalization_editor_triggered();
	void openMainWidget(QString path);

	private:
	QFileSystemModel *folderModel;
	Ui::mainWindow *ui;
	//QWidget *activeWidget;
	QWidget *activeWidget = nullptr;
	QWidget *setUpLocList(QString &path);
};
