#include "locstuff.h"
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QPointer>															//REMEMBER TO CHECK FOR MEMORY LEAKS!!!
#include <QMapIterator>
#include <QRegularExpression>

/*locstuff::locstuff(QObject *parent) : QObject(parent){

}*/

QString locSepetator = ":   ";

QMap<QString, QString>* parseLoc(QString &path){
	QFile file("."+path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug() << "Failed to open file" << path;
		return nullptr;
	}

	QTextStream in(&file);
	QMap<QString, QString> *locMap = new QMap<QString, QString>;			//This might cause a memory leak
	QString line;
	int lineNum = 99999;		//6 digits should be enough. That means a max of ~900000 lines.

	while(!in.atEnd()) {
		lineNum++;
		line = in.readLine();
		if(line == "l_english:") continue;
		if(line.trimmed().startsWith("#") || line.trimmed() == ""){
			locMap->insert(QString::number(lineNum)+QString("#comment"), line);		//oh no...
		} else{
			auto stuff = line.trimmed().split(QRegularExpression(":\\d "));	//what if a different number is used? or there is more than one instance?
			if(stuff.size() != 2){ //if it's 1, then it's either a comment or just blank space.
				qDebug() << "Parsing error at line" << lineNum-100000 << ". Make sure there is only one \":0\" on this line";
				locMap->insert(QString::number(lineNum)+QString("#error"), line);
				continue;
			}
			locMap->insert(QString::number(lineNum)+stuff[0], stuff[1]);
		}

		if(lineNum == 999999){			//only 6 numbers are used for each line. This prevents an overflow.
			qDebug() << "Localization file is too big, truncated at line 999999"; //It would be better if QMap was not sorted
			return locMap;
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
		if(i.key().sliced(6).startsWith("#")){
			out << i.value() << Qt::endl;
		} else{
			out << " " << i.key().sliced(6) << locSepetator << i.value() << Qt::endl;
		}
	};
	out.flush();
}

void unparseLoc(QTextStream &out, QMap<QString, QString> &locMap){	//exporting to stream, idk might need it
	out << "l_english:" << Qt::endl;
	QMapIterator<QString, QString> i(locMap);
	while(i.hasNext()) {
		i.next();
		if(i.key().sliced(6).startsWith("#")){
			out << i.value() << Qt::endl;
		} else{
			out << " " << i.key().sliced(6) << locSepetator << i.value() << Qt::endl;
		}
	}
}

void setUpLoc(QListWidget *activeWidget, QString &path){
	QMap<QString, QString> *locMap = parseLoc(path);

	QMapIterator<QString, QString> i(*locMap);
	while(i.hasNext()){
		i.next();
		if(i.key().sliced(6).startsWith("#")){
			activeWidget->addItem(i.value());
		} else{
			activeWidget->addItem(i.key().sliced(6) + locSepetator + i.value());
		}
	}
}
