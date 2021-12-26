#include "dataclass.h"


dataClass::dataClass(QObject *parent): QObject(parent){
	locAll = nullptr;
}

dataClass::~dataClass(){
	delete locAll;
}

void messHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg){
	QByteArray localMsg = msg.toLocal8Bit();
	const char *file = context.file ? context.file : "";
	const char *function = context.function ? context.function : "";
	switch (type) {
		case QtDebugMsg:	//not for release
			fprintf(stdout, "[ Debug ]: (%s:%u, %s)\n%s\n", file, context.line, function, localMsg.constData());
			break;

		case QtInfoMsg:		//Something that does not directly cause issues: parsing errors,
			//fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
			break;

		case QtWarningMsg:{	//Something that the user should be warned about: duplicate loc entry,
			//fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
			break;}

		case QtCriticalMsg:{	//A big problem, with an error popup: file can't be opened,
			fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
			QMessageBox errBox;
			errBox.setIcon(QMessageBox::Critical);
			errBox.setText("Critical error:");
			errBox.setInformativeText(localMsg.constData());
			errBox.exec();
			break;}

		case QtFatalMsg:	//most likely SEGFAULT lol
			fprintf(stderr, "[ Fatal ]: (%s:%u, %s)\n %s\n", file, context.line, function, localMsg.constData());
			break;
	}

	fflush(stdout);
}
