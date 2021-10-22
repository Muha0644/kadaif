#include "locstuff.h"
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QPointer>															//REMEMBER TO CHECK FOR MEMORY LEAKS!!!
#include <QMapIterator>
#include <QRegularExpression>

locstuff::locstuff(QObject *parent) : QObject(parent){

}

QMap<QString, QString>* parseLoc(QString &path){
	QFile file("."+path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug() << "Failed to open file" << path;
		return nullptr;
	}

	QTextStream in(&file);
	QMap<QString, QString> *locMap = new QMap<QString, QString>;			//This will cause a memory leak
	QString line;

	while(!in.atEnd()) {
		line = in.readLine();
		if(line.trimmed().startsWith("#")){
			locMap->insert(QString("#comment")+char(in.pos()), line);		//oh no...
		} else{
			auto stuff = line.trimmed().split(QRegularExpression(":\\d "));	//what if a different number is used? or there is more than one instance?
			if(stuff.size() != 2){
				qDebug() << "Parsing error at" << in.pos() << ". Make sure there is only one \":0\" on this line";
				locMap->insert(QString("#comment")+char(in.pos()), line);
				continue;
			}
			locMap->insert(stuff[0], stuff[1]);
		}
	}
	return locMap;
}

void unparseLoc(QString &path, QMap<QString, QString> &locMap){		//exporting to file
	QFile file("."+path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
		qDebug() << "Failed to open file" << path;
		return;
	}
	QTextStream out(&file);
	out.setEncoding(QStringConverter::Utf8);						//maybe unnecessarry?
	out.setGenerateByteOrderMark(true);

	out << "l_english:" << Qt::endl;
	QMapIterator<QString, QString> i(locMap);
	while(i.hasNext()){
		i.next();
		if(i.key().startsWith("#")){
			out << i.value() << Qt::endl;
		} else{
			out << " " << i.key() << ":0 " << i.value() << Qt::endl;
		}
	};
	out.flush();
}

void unparseLoc(QTextStream &out, QMap<QString, QString> &locMap){	//exporting to stream, idk might need it
	out << "l_english:" << Qt::endl;
	QMapIterator<QString, QString> i(locMap);
	while(i.hasNext()) {
		i.next();
		if(i.key().startsWith("#")){
			out << i.value() << Qt::endl;
		} else{
			out << " " << i.key() << ":0 " << i.value() << Qt::endl;
		}
	}
}

void setUpLoc(QListWidget *activeWidget, QString &path){
	QMap<QString, QString> *locMap = parseLoc(path);

	QMapIterator<QString, QString> i(*locMap);
	while(i.hasNext()){
		i.next();
		if(i.key().startsWith("#")){
			activeWidget->addItem(i.value());
		} else{
			activeWidget->addItem(i.key() + i.value());
		}
	}
}
