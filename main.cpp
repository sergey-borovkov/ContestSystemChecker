#include <QtCore>
#include <QDebug>

#include <iostream>

#include "submissionprocess.h"
#include "compiler.h"
#include "checker.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

    if(argc != 5)
	{
		qDebug() << "Syntax: checker submission_id task_number language_name path_to_source";
		return 1;
	}

	int submissionId = QString(argv[1]).toInt();
	QString taskNumber = QString(argv[2]);
	QString languageName(argv[3]);
	QString pathToSource(argv[4]);

    QString contestFolder = "C:/Disk_H/";
	QString pathToSubmissionFolder = contestFolder + "submissions/" + QString::number(submissionId) + '/';
	QDir testDirectory;
	if(!testDirectory.mkpath(pathToSubmissionFolder))
	{
        std::cout << "Unable to create folder " << (WCHAR *)pathToSubmissionFolder.constData();
		return 1;
	}

 	Compiler *c = makeCompiler(languageName);
	if(c == NULL)
	{
		std::cout << (WCHAR *) QString("Compiling %1 is not supported").arg(languageName).constData();
		return 1;
	}

	QString pathToProgram = pathToSubmissionFolder + QString::number(submissionId) + ".exe";

 	if(!c->compile(pathToSource, pathToProgram))
	{
		std::cout << "Compile error" << std::endl;
		return 0;
	}

	delete c;

    testDirectory.cd(pathToSubmissionFolder);

	testDirectory.cd(QString("../../tasks/%1").arg(taskNumber));

	QStringList tests = testDirectory.entryList(QStringList() << "*.in");
	tests.sort();
	QStringList answers = testDirectory.entryList(QStringList() << "*.ans");
	answers.sort();

	if(tests.size() != answers.size())
	{
		std::cout << "Number of tests must be equal number of output files";
		return 1;
	}

	for(int i = 0; i < tests.size(); i++)
	{
		tests[i] = testDirectory.absoluteFilePath(tests[i]);
		answers[i] = testDirectory.absoluteFilePath(answers[i]);
	}

//	std::cout << tests.toAscii().constData();
//	std::cout << answers.toAscii().constData();
	
	ProcessParameters p;
	p.workingDirectory = pathToSubmissionFolder;
	p.programPath = pathToProgram;
	p.memoryLimit = 1024 * 1024 * 256; 
	p.timeLimit = 2000;
	p.language = languageName;
//	SubmissionProcess pp(p);
//	pp.start();

	Checker checker(NULL, p, tests, answers, contestFolder, taskNumber);
	checker.startChecking();

	return a.exec();
}
