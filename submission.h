#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <QString>
#include <QDateTime>


struct Submission
{
public:
	Submission();
	Submission(QString string);
	~Submission();

//private:
	void parseString(const QString & string);
	int submissionId;
	QString sourceCode;
	QDateTime submissionTime;
};

#endif // SUBMISSION_H
