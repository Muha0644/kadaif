#include "loceditor.h"
#include "ui_loceditor.h"

locEditor::locEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::locViewer)
{
	ui->setupUi(this);
}

locEditor::~locEditor()
{
	delete ui;
}
