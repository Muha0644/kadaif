#include "locstuff.h"
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QRegularExpression>

/*locstuff::locstuff(QObject *parent) : QObject(parent){

}*/

QList<QString>* parseLocFile(QString &path){
	QFile file("."+path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug() << "Failed to open file" << path;
		return nullptr;
	}

	QTextStream in(&file);
	QList<QString> *fileLocList = new QList<QString>;
	QString line;

	while(!in.atEnd()) {
		line = in.readLine();
		//if(line == "l_english:") continue;		//maybe add other language support? nah, too much work
		fileLocList->append(line);
	}
	return fileLocList;
}

void loadLocFile(QListWidget *activeWidget, QString &path){
	QList<QString> *fileLocList = parseLocFile(path);

	QString entry;
	foreach(entry, *fileLocList){
		activeWidget->addItem(entry);
	}
	delete fileLocList;
}


QHash<QString, locEntry>* loadLoc(){
	QHash<QString, locEntry>* locAll = new QHash<QString, locEntry>;

	QDir locDir(".");
	if(!locDir.cd("localisation")){		//what if it doesn't exist?
		if(!locDir.mkdir("localisation")){
			qDebug() << "Failed to make \"localisation\" folder. Make sure you have write access to the mod folder.";
			//should abort now soooo
			delete locAll; //don't want to cause a memory leak...
			return nullptr;
		}
		locDir.cd("localisation");
	}

	QStringList files = locDir.entryList(QStringList() << "*.yml",QDir::Files);
	foreach(QString filename, files){	//open each file and add entries to the hashmap
		QFile file(filename);
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			qDebug() << "Failed to open file \"localisation/" << filename << "\".";
			//return nullptr;
		}
		QTextStream in(&file);
		QString line;
		int lineNum = 0;
		while(!in.atEnd()) {
			lineNum++;
			line = in.readLine();
			if(line == "l_english:" || line.trimmed().startsWith("#") || line.trimmed() == "") continue;

			auto stuff = line.trimmed().split(QRegularExpression(":\\d "));	//what if there is more than one instance?
			if(stuff.size() != 2){ //if it's 1, then it's either a comment or just blank space.
				qDebug() << "Loc parsing error in file" << filename << "at line" << lineNum << ". Make sure there is only one \":0\" on this line";
				continue;
			}
			locEntry yeah;
			yeah.file = filename;
			yeah.line = lineNum;
			yeah.key = stuff[0];			//is it really necessarry?
			yeah.value = stuff [1];
			locAll->insert(stuff[0], yeah);
		}
	}
	return locAll;
}

/*void unparseLoc(QString &path, QList<QString> &locFileList){
	QFile file("."+path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
		qDebug() << "Failed to open file" << path;
		return;
	}
	QTextStream out(&file);
	out.setEncoding(QStringConverter::Utf8);						//maybe unnecessarry?
	out.setGenerateByteOrderMark(true);

	out << "l_english:" << Qt::endl;
	QMapIterator<QString, QString> i(locFileList);
	while(i.hasNext()){
		i.next();
		if(i.key().sliced(6).startsWith("#")){
			out << i.value() << Qt::endl;
		} else{
			out << " " << i.key().sliced(6) << i.value() << Qt::endl;
		}
	};
	out.flush();
}
*/
