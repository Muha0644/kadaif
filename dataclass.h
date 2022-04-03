#pragma once
#include "config.h"
#include <loc/locstuff.h>
#include <gfx/gfxstuff.h>
#include <QHash>
#include <QString>
#include <QObject>
#include <QList>
#include <stdio.h>
#include <stdlib.h>

class dataClass: public QObject{ //my first singleton :)
	Q_OBJECT

public:
	static dataClass& getReference();

	QHash<QString, locEntry>* locAll;
	QHash<QString, gfxEntry>* gfxAll;

private:
	explicit dataClass(QObject *parent = nullptr);
	~dataClass();

signals:

};

void messHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
