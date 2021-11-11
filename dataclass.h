#pragma once
#include <QHash>
#include <QString>
#include <loc/locstuff.h>
#include <QObject>

class dataClass: public QObject{
	Q_OBJECT

public:
	explicit dataClass(QObject *parent = nullptr);
	~dataClass();

	QHash<QString, locEntry>* locAll;

signals:

};

