#pragma once
#include <QMainWindow>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE

class mainWindow : public QMainWindow{
	Q_OBJECT

	public:
	mainWindow(QWidget *parent = nullptr);
	~mainWindow();

	private slots:
	void on_openFolder_triggered();

	void on_splitter_splitterMoved(int pos, int index);

	void on_fileListThing_doubleClicked(const QModelIndex &index);

	private:
	Ui::mainWindow *ui;
	QWidget *activeWidget = new QRadioButton;
};
