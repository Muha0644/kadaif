#include "gfxeditor.h"
#include "ui_gfxeditor.h"
#include "gfxstuff.h"


gfxeditor::gfxeditor(const gfxEntry &entry, QWidget *parent): QDialog(parent), ui(new Ui::gfxeditor), entry{entry}{
	ui->setupUi(this);
	QSettings settings("muha0644","Kadaif");
	restoreGeometry(settings.value("gfxedit/geometry").toByteArray());

	pngvju = new PngView(pngify(entry.texturepath));
	ui->preview->addWidget(pngvju);

	QFont font(FONT_SERIF, 11);
	font.setFixedPitch(false);

	ui->nameEdit->setFont(font);
	ui->nameEdit->setText(entry.key);
	ui->texturefileEdit->setFont(font);
	ui->texturefileEdit->setText(entry.texturepath);

	setWindowTitle(entry.key);
}

gfxeditor::~gfxeditor(){
	QSettings settings("muha0644","Kadaif");
	settings.setValue("gfxedit/geometry", saveGeometry());
	delete ui;
}

void gfxeditor::on_nameEdit_textEdited(const QString &arg1){
	if(windowTitle() == ("* " + arg1)) return;
	setWindowTitle("* " + arg1);
}
void gfxeditor::on_texturefileEdit_textEdited(const QString &arg1){
	//pngvju->change(arg1);	//...remaking the WHOLE FUCKING OBJECT BECAUSE
	//CHANGING THE FUCKING IMAGE DOESN'T WORK!
	//WHY DOESN'T IT WANT TO WORK, I CAN MAKE ELIPSES BUT NOT PIXMAPS
	delete pngvju;
	pngvju = new PngView(pngify(arg1));
	ui->preview->addWidget(pngvju);

	if(windowTitle().startsWith("*")) return;
	setWindowTitle("* " + windowTitle());
}

void gfxeditor::on_Save_released(){
	gfxEntry newEntry;
	newEntry.file = entry.file;
	newEntry.line = entry.line;
	newEntry.texturepath = ui->texturefileEdit->text();
	newEntry.key = ui->nameEdit->text();
	saveAGfxEntry(newEntry, entry);
	emit saved("interface/" + entry.file);
	close();
}
