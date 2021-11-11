#include "loceditor.h"
#include "locstuff.h"
#include "ui_loceditor.h"
#include <QListWidget>

//STILL NO WAY TO EDIT KEYS!
//or a preview, which was my goal...

locHighlighter::locHighlighter(QTextDocument* parent):QSyntaxHighlighter(parent){
	this->isTool = 0;
	HighlightingRule rule;
}

locHighlighter::locHighlighter():QSyntaxHighlighter(new QTextDocument){	//sussy
	this->isTool = 1;
	HighlightingRule rule;

}

void locHighlighter::highlightBlock(const QString& text){

}

void locEditor::on_textEdit_textChanged(){
	if(windowTitle().startsWith("*")) return;
	setWindowTitle("* " + windowTitle());
}

locEditor::locEditor(QWidget *parent): QWidget(parent),	ui(new Ui::locEditor){		//opened as tool
	QSettings settings("muha0644","Kadaif");
	restoreGeometry(settings.value("locedit/geometry").toByteArray());
	ui->setupUi(this);

	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(false);
	font.setPointSize(11);

	ui->textEdit->setFont(font);

	locHigh = new locHighlighter(ui->textEdit->document());
	delete ui->saveButton;

	//ui->textEdit->setPlainText();
}

locEditor::locEditor(dataClass *liveDB,QListWidgetItem *item, QWidget *parent): QWidget(parent), ui(new Ui::locEditor){ //opened by double clicking an item
	QSettings settings("muha0644","Kadaif");
	restoreGeometry(settings.value("locedit/geometry").toByteArray());
	ui->setupUi(this);
	this->liveDB = liveDB;

	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(false);
	font.setPointSize(11);

	ui->textEdit->setFont(font);

	locHigh = new locHighlighter(ui->textEdit->document());

	entry.line = item->listWidget()->currentRow()+1;
	QString itemline = item->text();
	auto stuff = itemline.trimmed().split(QRegularExpression(":\\d "));
	if(stuff.size() != 2){
		ui->textEdit->setPlainText(itemline.trimmed());
		setWindowTitle("line "+QString::number(item->listWidget()->currentRow()+1));
		if(itemline.trimmed() == "" || itemline.trimmed().startsWith("#")){

			return;
		}
		qDebug() << "Error parsing loc item:" << item->text() << ". Make sure there is only one ':0' on this line";
		return; //maybe an error popup?
	} else{			//an actual entry has been found
		entry = liveDB->locAll->value(stuff[0]);

		//TODO: make it be nice maybe just add a preview window?
		//auto text = entry.value.split('"',Qt::SkipEmptyParts);
		//if(text.size() == 0){	//it's a comment... well not really, should look into this!
			ui->textEdit->setPlainText(entry.value);	//so the entry has " somewhere.
		//} else {
			//ui->textEdit->setPlainText(text[0]);
		//}
		setWindowTitle(entry.key);
	}
}

locEditor::~locEditor(){
	QSettings settings("muha0644","Kadaif");
	settings.setValue("locedit/geometry", saveGeometry());
	delete ui;
}

void locEditor::on_saveButton_clicked(){
	entry.value = ui->textEdit->toPlainText();
	if(entry.file == ""){
		return; //it should be copied to clipboard, but not now.
	}
	QString path = "/localisation/" + entry.file;
	QList<QString> *loceList = parseLocFile(path);
	if(entry.key == ""){
		loceList->replace(entry.line-1, entry.value);
	} else {
		loceList->replace(entry.line-1, " " + entry.key + ":0 " + entry.value);
	}
	saveLocFile(loceList, path);

	liveDB->locAll->insert(entry.key, entry);
	setWindowTitle(entry.key);
	emit saved(path);
	this->destroy();
}


void locEditor::on_copyButton_clicked(){
	ui->textEdit->selectAll();
	ui->textEdit->copy();
	ui->textEdit->moveCursor(QTextCursor::Start);
}

