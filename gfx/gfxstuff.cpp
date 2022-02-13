#include "gfxstuff.h"
#include "config.h"
#include <QDirIterator>
#include <QFile>

QHash<QString, gfxEntry>* loadGfx(){
	QHash<QString, gfxEntry>* gfxAll = new QHash<QString, gfxEntry>;
	QSettings settings("muha0644", "Kadaif");
	settings.setValue("gfxDup", "");

	QDir gfxDir(".");
	if(!gfxDir.cd("interface")){		//what if it doesn't exist?
		if(!gfxDir.mkdir("interface")){
			qCritical() << "Failed to create \"interface\" folder. Something is seriously wrong.";
			delete gfxAll; //abortus
			return nullptr;
		}
		gfxDir.cd("interface");
	}

	QDirIterator iter("./interface", {"*.gfx"}, QDir::Files ,QDirIterator::Subdirectories);
	while(iter.hasNext()){
		QString filepath = iter.next();
		QFile file(filepath);
		int liNo = 0;
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			qCritical() << "Failed to open gfx file" << filepath.remove("./interface/") << ":" <<file.errorString();
			continue;
		}
		QTextStream in(&file);
		QString line = "#";
		while(line.startsWith("#")){
			line = in.readLine().trimmed(); liNo++;
		}
		if(!line.startsWith("spriteTypes = {")){
			qInfo() << "Parsing error in file " + filepath + ". This is not a valid gfx file.";
			continue;
		}

		gfxEntry* entry = nullptr;
		bool isSpriteType = false;
		while(!in.atEnd()){
			line = in.readLine().trimmed(); liNo++;
			while(line.startsWith("#") || line.trimmed() == ""){ //skip comments
				line = in.readLine().trimmed(); liNo++;
			}
			if(isSpriteType){	//are you inside the nested brackets (IE. inside spriteTypes = {...} ) rn?
				if(line.startsWith("name = ", Qt::CaseInsensitive)){
					if(line.count('"') != 2){
						qInfo() << "Invalid (empty) spriteType name in " + entry->file + ", line " + QString::number(liNo);
						continue;
					}
					entry->key = line.section('"', 1).remove('"');
				} else if(line.startsWith("texturefile = ", Qt::CaseInsensitive)){
					if(line.count('"') != 2){
						qInfo() << "Invalid (empty) filepath in " + entry->file + ", line " + QString::number(liNo);
						continue;
					}
					entry->texturepath = line.section('"', 1).remove('"');
				} else if(line.startsWith("}")){ //sprite has been declared correctly
					isSpriteType = false;
					if(gfxAll->contains(entry->key)){	//duplicate check
						QString mesg("Duplicate key found: " + entry->key + "\n" + entry->file + " on line: " + QString::number(entry->line) + "\n"
								+ gfxAll->value(entry->key).file + " on line: "  + QString::number(gfxAll->value(entry->key).line));
						qWarning() << mesg;
						settings.setValue("gfxDup", settings.value("gfxDup").toString() + mesg + "\n\n");
					}
					gfxAll->insert(entry->key, *entry);
					delete entry;
					continue;
				} else {	//what else could be here? lazyloading?
					//!!TODO lazyloading stuff goes here
					qInfo() << "Parsing error in file " + filepath + ": unexpected line in SpriteType decleration on line " + QString::number(liNo)
							<< "ignore this if it's about lazy loading";
				}
			} else {//expecting sprite decleration or clsoing brackets
				if(line.startsWith("spriteType = {")){
					entry = new gfxEntry;
					entry->line = liNo;
					entry->file = filepath.remove("./interface/");
					isSpriteType = true;
					continue;
				}
				if(line.startsWith("}")) break;	//file about to end
				//qInfo() << "Parsing error in file " + filepath + ": expected SpriteType decleration or '}' on line " + QString::number(liNo);
			}
		}
	}
	return gfxAll;
}
