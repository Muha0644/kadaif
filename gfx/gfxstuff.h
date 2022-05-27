#pragma once
#include "config.h"
#include <QString>
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSettings>
#include <QHash>
#include <OpenImageIO/imageio.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QKeyEvent>

struct gfxEntry{
	QString file;	//path to the file this entry is from (omits "interface/")
	int line;		//index 0 is file header
	QString key;	//empty key means section header/long comment
	QString texturepath;
};


void loadGfxFile(QHash<QString, gfxEntry>* gfxAll, QString &filepath);
QHash<QString, gfxEntry>* loadGfx();
void loadGfxEntries(QListWidget *content, QString &path);

QString pngify(QString& path);	//takes a path to a texture and returns the path of a temp png version (with refresh in mind), "" if path does not exist

class gfxWidget: public QWidget{
	Q_OBJECT
	public:
	explicit gfxWidget(QString &path, QObject *parent = nullptr);
	~gfxWidget();

	signals:

};

class PngView: public QGraphicsView{
	Q_OBJECT
	public:
	explicit PngView(const QString& pngPath, QWidget *parent = nullptr);
	protected Q_SLOTS:
	void wheelEvent(QWheelEvent *event);
};
