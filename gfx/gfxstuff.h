#pragma once
#include "config.h"
#include <QString>
#include <QSettings>
#include <QHash>

struct gfxEntry{
	QString file;	//path to the file this entry is from (omits "interface/")
	int line;		//index 0 is file header
	QString key;	//empty key means section header/long comment
	QString texturepath;
};


QHash<QString, gfxEntry>* loadGfx();
void tryFix(const QString& path);
