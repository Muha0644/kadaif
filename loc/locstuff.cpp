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
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qCritical() << "Failed to open file" << path << "for parsing.";
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

void saveLocFile(QList<QString> *list, QString& path){
		QFile file(path);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
			qCritical() << "Failed to open file" << path << "for writing.";
			return;
		}
		QTextStream out(&file);
		out.setEncoding(QStringConverter::Utf8);						//maybe unnecessarry?
		out.setGenerateByteOrderMark(true);

		foreach(QString line, *list){
			out << line << Qt::endl;
		}
		out.flush();
}

QHash<QString, locEntry>* loadLoc(){
	QHash<QString, locEntry>* locAll = new QHash<QString, locEntry>;
	QSettings settings("muha0644","Kadaif");
	settings.setValue("locDup", "");

	QDir locDir(".");												//!!!!!!!!!
	if(!locDir.cd("localisation")){		//what if it doesn't exist?
		if(!locDir.mkdir("localisation")){
			qCritical() << "Failed to create \"localisation\" folder. Something is seriously wrong.";
			//should abort now soooo
			delete locAll; //don't want to cause a memory leak...
			return nullptr; //maybe it's better to just return an empty QHash?
		}
		locDir.cd("localisation");
	}

	QStringList files = locDir.entryList(QStringList() << "*.yml",QDir::Files);
	foreach(QString filename, files){	//open each file and add entries to the hashmap
		QFile file(locDir.absoluteFilePath(filename));
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			qCritical() << "Failed to open file localisation file" << filename << ":" <<file.errorString();
			//return nullptr;
			continue;
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
				qInfo() << "Loc parsing error in file" << filename << "at line" << lineNum << ". Make sure there is only one ':0' on this line";
				continue;
			}
			locEntry yeah;
			yeah.file = filename;
			yeah.line = lineNum;
			yeah.key = stuff[0];
			yeah.value = stuff [1];
			if(locAll->contains(stuff[0])){
				QString mesg("Duplicate key found: " + stuff[0] + "\n" + yeah.file + " on line: " + QString::number(yeah.line) + "\n"
						+ locAll->value(stuff[0]).file + " on line: "  + QString::number(locAll->value(stuff[0]).line));
				qWarning() << mesg;
				settings.setValue("locDup", settings.value("locDup").toString() + mesg + "\n\n");
			}
			locAll->insert(stuff[0], yeah);
		}
	}
	return locAll;
}

