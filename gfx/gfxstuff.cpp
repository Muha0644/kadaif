#include "gfx/gfxstuff.h"
#include "config.h"
#include "gfx/gfxeditor.h"
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
	while(!in.atEnd()){		//reflective comment: damn i used to suck at programming...
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
			} else {
				QList<QString> valid = {"legacy_lazy_load", "allwaystransparent", "transparencecheck", "effectFile", "noOfFrames"};
				if(valid.contains(line.first(line.indexOf("=")).trimmed(), Qt::CaseInsensitive)) continue;
				if(line.startsWith("animation = {")){
					while(!line.startsWith("}")){
						line = in.readLine().trimmed(); liNo++;
					}
					continue;
				}
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
			return new QHash<QString, gfxEntry>; //don't return nullptr
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
#if !NO_TITLE
	vLayout->setContentsMargins(5, 2, 0, 0);
	QLabel *title = new QLabel(path);
	QFont font(FONT_SANS, 12, QFont::Bold);
	title->setFont(font);
	vLayout->addWidget(title);
#endif

	gfxList = new QListWidget();
	connect(newButt, &QPushButton::clicked, this, &gfxWidget::newthis);
	connect(deleteButt, &QPushButton::clicked, this, &gfxWidget::deletethis);
	connect(editButt, &QPushButton::clicked, this, &gfxWidget::edit);
	connect(gfxList, &QListWidget::itemDoubleClicked, this, &gfxWidget::edit);
	buttonBar->addWidget(newButt);
	buttonBar->addWidget(deleteButt);
	buttonBar->addWidget(editButt);
	buttonBar->addSpacerItem(spase);

	curPath = path;
	refreshList();
	vLayout->addLayout(buttonBar);
	vLayout->addWidget(gfxList);
}
gfxWidget::~gfxWidget(){
	delete gfxList;
}

void gfxWidget::refreshList(){
	gfxList->clear();
	loadGfxEntries(gfxList, curPath);
}

void gfxWidget::edit(){
	if(gfxList->currentRow() == -1) return;
	gfxeditor *gfxEditW = new gfxeditor(liveDB.gfxAll->value(gfxList->currentItem()->text()));
	gfxEditW->setAttribute(Qt::WA_DeleteOnClose);
	gfxEditW->setAttribute( Qt::WA_QuitOnClose, false);
	gfxEditW->show();
	connect(gfxEditW, &gfxeditor::saved, this, &gfxWidget::refreshList);
}
void gfxWidget::deletethis(){
	if(gfxList->currentRow() == -1) return;//-1 means nothing is selected
	deleteAGfxEntry(liveDB.gfxAll->value(gfxList->currentItem()->text()));
	refreshList(); //usually editor calls this
}
void gfxWidget::newthis(){
	gfxList->insertItem(gfxList->currentRow()+1, " ");
	gfxEntry newEntry;
	newEntry.file = curPath.sliced(sizeof("interface")); //should find a better way of doing this...
	newEntry.line = -(liveDB.gfxAll->value(gfxList->currentItem()->text())).line; //negative number means insert new entry after that line
	gfxList->setCurrentRow(gfxList->currentRow()+1);

	gfxeditor *gfxEditW = new gfxeditor(liveDB.gfxAll->insert(" ", newEntry).value());
	gfxEditW->setAttribute(Qt::WA_DeleteOnClose);
	gfxEditW->setAttribute( Qt::WA_QuitOnClose, false);
	gfxEditW->show();
	connect(gfxEditW, &gfxeditor::saved, this, &gfxWidget::refreshList);
}
void gfxWidget::dup(){
	QSettings settings("muha0644", "Kadaif");
	QMessageBox msgBox;
	msgBox.setText("Duplicate gfx entries:");
	msgBox.setInformativeText(settings.value("gfxDup").toString());
	msgBox.exec();
}
void gfxWidget::empty(){
	QString empty;
	foreach(auto entry, liveDB.gfxAll->values()){
		if(entry.texturepath.trimmed().isEmpty()){
			empty.append("Empty entry found: " + entry.key + "\nin " + entry.file + " near line " + QString::number(entry.line) + "\n\n");
		};
	}
	QMessageBox msgBox;
	msgBox.setText("Empty gfx entries:");
	msgBox.setInformativeText(empty);
	msgBox.exec();
}

void loadGfxEntries(QListWidget *content, QString &path){	//loads entires from file into a list with icons. Should be a method of gfxWidget but whatever...
	QHash<QString, gfxEntry>* gfxEntries = new QHash<QString, gfxEntry>;
	loadGfxFile(gfxEntries, path);

	QHashIterator<QString, gfxEntry> iter(*gfxEntries);
	QMap<int, gfxEntry> sorted;
	while(iter.hasNext()){
		gfxEntry entry = iter.next().value();
		sorted.insert(entry.line, entry);
	}
	delete gfxEntries;

	QMap<int, gfxEntry>::const_iterator i = sorted.constBegin();	//trying out STL-style iterator, instead of java-style
	while (i != sorted.constEnd()) {
		gfxEntry entry = i.value();
		//open the .dds with OIIO and export it to png into a temp dir
		//you don't need to hear the rant i put here before...
		//tl;dr Qt had .dds support before, now it doesn't.
		QString pngpath = pngify(entry.texturepath);
		if(pngpath == "")  qInfo() << "Texture file" << entry.texturepath << "defined near" << entry.file+":"+QString::number(entry.line) << "does not exist.";
		content->addItem(new QListWidgetItem(QIcon(QPixmap(pngpath)), entry.key));
		++i;
	}// Conclusion: java-style iterators are slower but nicer.
}

void saveAGfxEntry(const gfxEntry &newEntry, const gfxEntry &oldEntry){	//this approach is a bit more surgical than the loc code
	QFile file("interface/" + newEntry.file);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qCritical() << "Failed to open gfx file" << newEntry.file << "for reading";
		return;
	}

	QString line, buffer;
	QTextStream stream(&file);
	if(oldEntry.line > 0){ // negative number means add new entry
		for(int i = 0; i < oldEntry.line; i++){	// skip to the important part
			buffer.append(stream.readLine()+NEWLINE);
		}// should be at the line with "spriteType = {"
		while(!line.contains("}")){// loop until the end of the entry
			stream.readLineInto(&line);
			if(line.trimmed().startsWith("name = ", Qt::CaseInsensitive)){
				line.replace(oldEntry.key, newEntry.key);
			} else if(line.trimmed().startsWith("texturefile = ", Qt::CaseInsensitive)){
				line.replace(oldEntry.texturepath, newEntry.texturepath);
			}
			buffer.append(line+NEWLINE);
		}
		while(!stream.atEnd()){//found and replaced the important part, finish buffering the file
			buffer.append(stream.readLine()+NEWLINE);
		}
	} else { //adding a new entry to the end.
		for(int i = 1; i < -oldEntry.line; i++) buffer.append(stream.readLine()+NEWLINE);
		buffer.append("\tspriteType = {" NEWLINE); //do not question why this works. It does.
		buffer.append("\t\tname = \"" + newEntry.key + "\"" NEWLINE);
		buffer.append("\t\ttexturefile = \"" + newEntry.texturepath + "\"" NEWLINE);
		buffer.append("\t}" NEWLINE);
		while(!stream.atEnd()) buffer.append(stream.readLine()+NEWLINE);
	}

	file.close();			//write buffer to stream
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	file.write(buffer.toUtf8());
	file.close();			//since we know what entry was changed, no need to reload the entire database
	dataClass &liveDB = dataClass::getReference();
	liveDB.gfxAll->remove(oldEntry.key);
	liveDB.gfxAll->insert(newEntry.key, newEntry);
}

void deleteAGfxEntry(const gfxEntry& entry){
	QFile file("interface/" + entry.file);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qCritical() << "Failed to open gfx file" << entry.file << "for reading & writing.";
		return;
	}
	QString line, buffer;
	QTextStream stream(&file);

	for(int i = 1; i < entry.line; i++){
		buffer.append(stream.readLine()+NEWLINE);
	}// should be before the line with "spriteType = {"
	while(!line.contains("}")) stream.readLineInto(&line);//not writing the entry to be deleted into the buffer
	while(!stream.atEnd()) buffer.append(stream.readLine()+NEWLINE);

	file.close();			//opening with r+w is buggy for some reason, so i have to open the file two times
	file.open(QIODevice::WriteOnly | QIODevice::Text); file.write(buffer.toUtf8()); file.close();
	dataClass &liveDB = dataClass::getReference();
	liveDB.gfxAll->remove(entry.key);
}

QString pngify(const QString& path){
	if(path.endsWith(".png")) return path;
	QString ddspath = "./" + path;
	QString tmppath = "./"  + path + ".png";

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
		QProcess::execute("magick", {ddspath, tmppath});
	} else return "";
	return tmppath;
}

PngView::PngView(const QString& pngPath, QWidget *parent): QGraphicsView(parent){
	setDragMode(QGraphicsView::ScrollHandDrag);

	scene = new QGraphicsScene;
	item = scene->addPixmap(QPixmap(pngPath));//new QGraphicsPixmapItem(QPixmap(pngPath));
	//at least i'm not heap allocating... easier to delete...
#if !PIXELART
	item->setTransformationMode(Qt::SmoothTransformation);
#endif
	setScene(scene);
}
void PngView::change(const QString& pngPath){//does not work. will not work. will not try to make it work.
	//jusit remake the whole thing, it's faster and easier.
	scene->clear();
	QPixmap *mm = new QPixmap(pngPath);
	item = scene->addPixmap(*mm);
#if !PIXELART
	item->setTransformationMode(Qt::SmoothTransformation);
#endif
	scene->update();
}
void PngView::wheelEvent(QWheelEvent *event){
	float scaleRN = transform().m11();
	if(event->angleDelta().y() > 0 ){
	  if(scaleRN < MAX_SCALE)   scale(SCALE_FACTOR, SCALE_FACTOR);
	}
	else{
		if(scaleRN > MIN_SCALE) scale(1/SCALE_FACTOR, 1/SCALE_FACTOR);
	}
}
