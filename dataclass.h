#pragma once
#include "config.h"
#include <QHash>
#include <QString>
#include <loc/locstuff.h>
#include <QObject>
#include <QList>
#include <stdio.h>
#include <stdlib.h>

class dataClass: public QObject{
	Q_OBJECT

public:
	explicit dataClass(QObject *parent = nullptr);
	~dataClass();

	QHash<QString, locEntry>* locAll;

signals:

};

void messHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
