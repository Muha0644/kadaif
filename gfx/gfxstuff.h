#pragma once
#include "dataclass.h"
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

/*struct gfxEntry{
	QString file;	//path to the file this entry is from (omits "interface/")
	int line;		//index 0 is file header
	QString key;	//empty key means section header/long comment
	QString texturepath;
};
 ^ defined in dataclass.h because of a broken cyclic dependency */

void loadGfxFile(QHash<QString, gfxEntry>* gfxAll, QString &filepath);
QHash<QString, gfxEntry>* loadGfx();
void loadGfxEntries(QListWidget *content, QString &path);
void saveAGfxEntry(const gfxEntry &newEntry, const gfxEntry &oldEntry);

QString pngify(const QString& path);	//takes a path to a texture and returns the path of a temp png version (with refresh in mind), "" if path does not exist

class gfxWidget: public QWidget{
	Q_OBJECT
	public:
	explicit gfxWidget(QString &path, QObject *parent = nullptr);
	~gfxWidget();

	void edit();
	void deletethis();	//new and delete already taken...
	void newthis();
	void dup();
	void empty();

	private:
	dataClass& liveDB = dataClass::getReference();
	QListWidget *gfxList;

	public slots:
	void refreshList(QString path);
};

class PngView: public QGraphicsView{
	Q_OBJECT
	public:
	explicit PngView(const QString& pngPath, QWidget *parent = nullptr);
	void change(const QString& pngPath);
	QGraphicsScene* scene;
	QGraphicsPixmapItem* item;

	protected Q_SLOTS:
	void wheelEvent(QWheelEvent *event);
};
