#pragma once

#include <QWidget>

namespace Ui {
	class locViewer;
}

class locEditor : public QWidget
{
	Q_OBJECT

	public:
	explicit locEditor(QWidget *parent = nullptr);
	~locEditor();

	private:
	Ui::locViewer *ui;
};

