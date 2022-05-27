#include "gfxstuff.h"
#include "config.h"
#include <QDirIterator>
#include <QScrollArea>
#include <QFile>
#include <QProcess>

void loadGfxFile(QHash<QString, gfxEntry>* gfxAll, QString &filepath){	//loads only one gfx file
	QFile file(filepath);
	int liNo = 0;
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qCritical() << "Failed to open gfx file" << filepath.remove("./interface/") << ":" <<file.errorString();
		return;
	}
	QTextStream in(&file);
	QString line = "#";
	while(line.startsWith("#") || (line.trimmed() == "")){
		line = in.readLine().trimmed(); liNo++;
	}
	if(!line.startsWith("spriteTypes = {")){
		qInfo() << "Parsing error in file " + filepath + ". This is not a valid gfx file.";
		return;
	}

	gfxEntry* entry = nullptr;
	bool isSpriteType = false;
	while(!in.atEnd()){
		line = in.readLine().trimmed(); liNo++;
		while(line.startsWith("#") || line.trimmed() == ""){ //skip comments
			line = in.readLine().trimmed(); liNo++;
		}
		line = line.section("#",0,0);	//ignore inline comments
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
					QSettings settings("muha0644", "Kadaif");
					settings.setValue("gfxDup", settings.value("gfxDup").toString() + mesg + "\n\n");
				}
				gfxAll->insert(entry->key, *entry);
				delete entry;							//wait what?? Apparently it works...
				continue;
			} else {	//what else could be here? lazyloading?
				//!!TODO lazyloading stuff goes here
				//! noOfFrames
				//! transparencecheck
				qInfo() << "Parsing error in file " + filepath + ": unexpected line in SpriteType decleration on line " + QString::number(liNo);
			}
		} else {//expecting sprite decleration or clsoing brackets
			if(line.startsWith("spriteType = {", Qt::CaseInsensitive)){
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

QHash<QString, gfxEntry>* loadGfx(){	//loads all the gfx
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
		loadGfxFile(gfxAll, filepath);
	}
	return gfxAll;
}

gfxWidget::gfxWidget(QString &path, QObject* parent){
	QVBoxLayout *vLayout = new QVBoxLayout(this);
	QHBoxLayout *buttonBar = new QHBoxLayout;
	QPushButton *editButt = new QPushButton("Edit");
	QPushButton *newButt = new QPushButton("New GFX entry");
	QPushButton *deleteButt = new QPushButton("Delete selection");
	QSpacerItem *spase = new QSpacerItem(10000,20,QSizePolicy::Preferred);
#ifndef NoTitle
	vLayout->setContentsMargins(5, 2, 0, 0);
	QLabel *title = new QLabel(path);
	QFont font;
	font.setBold(true);
	font.setPointSize(12);
	title->setFont(font);
	vLayout->addWidget(title);
#else
	vLayout->addWidget(new QWidget); // avoid segfault with this one simple trick!
#endif

	QListWidget *content = new QListWidget();
	/*
	connect(newButt, &QPushButton::clicked, this, &mainWindow::nononononew);
	connect(deleteButt, &QPushButton::clicked, this, &mainWindow::nonononodelete);
	connect(editButt, &QPushButton::clicked, this, &mainWindow::nonononoedit);
	connect(locListWidget, &QListWidget::itemDoubleClicked, this, &mainWindow::nonononoedit);
	*/

	buttonBar->addWidget(newButt);
	buttonBar->addWidget(deleteButt);
	buttonBar->addWidget(editButt);
	buttonBar->addSpacerItem(spase);

	loadGfxEntries(content, path);
	vLayout->addLayout(buttonBar);
	vLayout->addWidget(content);
}

gfxWidget::~gfxWidget(){

}

/*//not done
QList<gfxEntry>* parseGfxEntries(QString &path){
	QHash<QString, gfxEntry>* gfxEntries = new QHash<QString, gfxEntry>;
	loadGfxFile(gfxEntries, path);

	QList<gfxEntry> *fileGfxList = new QList<gfxEntry>;	//a gfx list from one file

	QHashIterator<QString, gfxEntry> iter(*gfxEntries);
	while(iter.hasNext()){
		gfxEntry gfe = iter.next().value();
		qDebug() << gfe.key << gfe.file << gfe.line << gfe.texturepath << "\n\n";
	}
	return fileGfxList;
}*/

void loadGfxEntries(QListWidget *content, QString &path){
	QHash<QString, gfxEntry>* gfxEntries = new QHash<QString, gfxEntry>;
	loadGfxFile(gfxEntries, path);

	QHashIterator<QString, gfxEntry> iter(*gfxEntries);
	while(iter.hasNext()){
		gfxEntry entry = iter.next().value();
		/*QVBoxLayout* frame = new QVBoxLayout;		//todo: improve this shit
		QPixmap pixmap("gfx/HOI4_icon.bmp");		//i want bigger icons and double spacing
		QLabel *icon = new QLabel;					//maybe try a form layout or a QListView?
		icon->setAlignment(Qt::AlignHCenter);
		icon->setPixmap(pixmap);
		QLabel *title = new QLabel(entry.key);
		title->setAlignment(Qt::AlignHCenter);
		title->setMaximumWidth(150);				//also add threading, large files will suck
		title->setMaximumHeight(48);
		title->setWordWrap(true);
		frame->addWidget(icon);
		frame->addWidget(title);*/

		//open the .dds with OIIO and export it to png into a temp dir
		//honestly fuck the qt team, they are all assholes. First they use a shitty licence
		//and then they just remove random features. They say "you can compile the plugin
		//yourself if you relly need the feature" but it doesn't fucking work!
		//The cmake moves some files around but you still can't open dds, so i have to
		//use this fucking mess and add a whole new dependency just to do something that
		//would have worked in an older version of Qt. AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		QString pngpath = pngify(entry.texturepath);
		if(pngpath == "")  qInfo() << "Texture file" << entry.texturepath << "defined near" << entry.file+":"+QString::number(entry.line) << "does not exist.";
		content->addItem(new QListWidgetItem(QIcon(QPixmap(pngpath)), entry.key));
	}
}

//not done
void saveAGfxEntry(QList<QString> *list, QString& path){	//this approach is a bit more surgical than the loc code
		QFile file(path);
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text)){
			qCritical() << "Failed to open file" << path << "for readingwriting.";
			return;
		}


		QTextStream out(&file);
		out.setEncoding(QStringConverter::Utf8);
		foreach(QString line, *list){
			out << line << Qt::endl;
		}
		out.flush();
}

QString pngify(QString& path){
	if(path.endsWith(".png")) return path;
	QString ddspath = QDir::currentPath() + "/" + path;
	QString tmppath = QDir::tempPath() + "/" + path + ".png";

	if(QFile::exists(tmppath)){
		if(QFileInfo(ddspath).lastModified() < QFileInfo(tmppath).lastModified()){
			return tmppath;
		}
	} else {
		QFileInfo file(tmppath);
		QDir dir;
		dir.mkpath(file.dir().path());
	}
	if(!path.isEmpty() && QFileInfo(ddspath).isFile()){
		QProcess::execute("iconvert", {ddspath, tmppath});
	} else return "";
	return tmppath;
}


PngView::PngView(const QString& pngPath, QWidget *parent): QGraphicsView(parent){
	setDragMode(QGraphicsView::ScrollHandDrag);

	QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap(pngPath));
	pixmapItem->setTransformationMode(Qt::SmoothTransformation);

	QGraphicsScene *scene = new QGraphicsScene();
	scene->addItem(pixmapItem);
	setScene(scene);
}

void PngView::wheelEvent(QWheelEvent *event){	//maybe limit max zoom?
	if(event->angleDelta().y() > 0)
	  scale(1.25, 1.25);
	else
	  scale(0.8, 0.8);
}
