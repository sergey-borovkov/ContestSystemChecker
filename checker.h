#ifndef CHECKER_H
#define CHECKER_H

#include <QtCore>
#include <QObject>

#include "submissionprocess.h"

class Checker : public QObject
{
	Q_OBJECT

public:
	Checker(QObject *parent, const ProcessParameters &parameters, QStringList & inputs, QStringList & answers, const QString & contestFolder, QString taskNumber);
	~Checker();
	void startChecking();

private slots:
	void processnextSubmit();
	void checkSubmitStatus(int);
private:

	struct Tests
	{
		Tests(const QStringList & tests, const QStringList & answers) : tests(tests), answers(answers) {}
		bool empty() { return answers.size() == 0; }
		QStringList answers;
		QStringList tests;
	};

	const ProcessParameters	&parameters;
	SubmissionProcess process;
	Tests tests;
	QString contestFolder;

	QString currentInputFile;
	QString currentAnswerFile;
	int testNumber;
	QString taskNumber;
};

#endif // CHECKER_H
