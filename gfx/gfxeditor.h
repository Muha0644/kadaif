#pragma once
#include <QDialog>
#include "gfxstuff.h"

namespace Ui {
	class gfxeditor;
}

class gfxeditor : public QDialog{
	Q_OBJECT

	public:
	explicit gfxeditor(const gfxEntry &entry,QWidget *parent = nullptr);
	~gfxeditor();

	private slots:
	void on_nameEdit_textEdited(const QString &arg1);
	void on_texturefileEdit_textEdited(const QString &arg1);
	void on_Save_released();

	signals:
	void saved(QString path);


	private:
	PngView* pngvju;
	gfxEntry entry;
	Ui::gfxeditor *ui;
};

