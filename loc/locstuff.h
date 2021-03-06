#pragma once
#include <QObject>
#include <QMap>
#include <QString>
#include <QtWidgets>
#include <QTextStream>
#include <QHash>


struct locEntry{
	QString file;
	int line;
	QString key;
	QString value;
};

QList<QString>* parseLocFile(QString &path);
void loadLocFile(QListWidget *activeWidget, QString &path);
void saveLocFile(QList<QString> *list, QString& path);
QHash<QString, locEntry>* loadLoc();
void unparseLoc(QString &path, QList<QString> &locFileList);
