#include "submission.h"

#include <QRegExp>
#include <QProcess>

Submission::Submission()
{

}

Submission::Submission( QString string )
{
	//parseString(string);
	sourceCode = "int main() {return 0;}";
	submissionId = 1;

}

Submission::~Submission()
{

}

void Submission::parseString(const QString & string)
{
	QRegExp regExp("id: ");	
	int submissionIdIndex = string.indexOf("id:");
//	submissionIdIndex = QString::number(string.mid());

}
