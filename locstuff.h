#pragma once
#include <QObject>
#include <QMap>
#include <QString>
#include <QtWidgets>
#include <QTextStream>

class locstuff: public QObject{
	Q_OBJECT
	public:
	explicit locstuff(QObject *parent = nullptr);

	signals:

};

QMap<QString, QString>* parseLoc(QString &path);

void unparseLoc(QString &path, QMap<QString, QString> &locMap);
void unparseLoc(QTextStream &out, QMap<QString, QString> &locMap);
void setUpLoc(QListWidget *activeWidget, QString &path);
