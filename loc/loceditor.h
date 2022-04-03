#pragma once
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QWidget>
#include <QList>
#include "dataclass.h"
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {class locEditor;}
QT_END_NAMESPACE

/*class locHighlighter: public QSyntaxHighlighter{
	Q_OBJECT

	public:
	explicit locHighlighter(QTextDocument *parent);
	locHighlighter();

	protected:
	void highlightBlock(const QString &text) override;

	private:
	bool isTool;
	struct HighlightingRule{
		QRegularExpression pattern;
		QTextCharFormat format;
	};
	QList<HighlightingRule> highlightingRules;

	QRegularExpression commentStartExpression;
	QRegularExpression commentEndExpression;

	QTextCharFormat keywordFormat;
	QTextCharFormat classFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat functionFormat;
};*/

class locEditor : public QWidget{
	Q_OBJECT

	public:
	locEditor(QWidget *parent = nullptr);
	locEditor(QListWidgetItem *item, QWidget *parent = nullptr);
	~locEditor();

	locEntry entry;
	dataClass& liveDB = dataClass::getReference();

	private slots:
	void on_textEdit_textChanged();
	void on_saveButton_clicked();
	void on_copyButton_clicked();

	signals:
	void saved(QString path);

	private:
	Ui::locEditor *ui;
	//locHighlighter *locHigh;
};


