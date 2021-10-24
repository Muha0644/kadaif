#pragma once
#include <QMainWindow>
#include <QtWidgets>
#include <QLayout>
#include "locstuff.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE

class mainWindow : public QMainWindow{
	Q_OBJECT

	public:
	mainWindow(QWidget *parent = nullptr);
	~mainWindow();
	QHash<QString, locEntry>* locAll;

	private slots:
	void on_openFolder_triggered();
	void on_splitter_splitterMoved(int pos, int index);
	void on_fileListThing_doubleClicked(const QModelIndex &index);
	void on_actionLocalization_editor_triggered();

	private:
	QFileSystemModel *folderModel;
	Ui::mainWindow *ui;
	//QWidget *activeWidget;
	QWidget *activeWidget = nullptr;
	QWidget *setUpLocList(QString &path);
};
