#include "checker.h"

#include <iostream>

Checker::Checker(QObject *parent, const ProcessParameters &parameters, QStringList & inputs, QStringList & answers, const QString & contestFolder, QString taskNumber)
	: QObject(parent), parameters(parameters), tests(inputs, answers), process(parameters), contestFolder(contestFolder), taskNumber(taskNumber)
{
	connect(&process, SIGNAL(finished(int)), SLOT(checkSubmitStatus(int)));
	testNumber = 0;
}

void Checker::startChecking()
{
	processnextSubmit();
}

void Checker::processnextSubmit()
{
	if(tests.empty())
	{
		std::cout << "AC" << std::endl;
		exit(0);
	}

	++testNumber;

	currentInputFile = tests.tests.takeAt(0);
	currentAnswerFile = tests.answers.takeAt(0);

	QString input = parameters.workingDirectory + "input.txt";
	QString output = parameters.workingDirectory + "output.txt";

	if(QFile::exists(input))
		QFile::remove(input);
	if(QFile::exists(output))
		QFile::remove(output);

	QFile file(currentInputFile);
	file.copy(input);
	process.start();
}

void Checker::checkSubmitStatus(int status)
{
	process.timeLapsed = 0;
	if(status == SubmissionProcess::MemoryLimit)
	{
		std::cout << "Test "<< testNumber << ":\nMemory Limit" << std::endl;
		exit(0);
	}
	else if(status == SubmissionProcess::TimeLimitError)
	{
		std::cout << "Test "<< testNumber << ":\nTime Limit" << std::endl;
		exit(0);
	}

	else if(status == SubmissionProcess::ProgramStartError)
	{
		std::cout << "Error" << std::endl;
		exit(0);
	}

	else if(status == SubmissionProcess::RunTimeError)
	{
		std::cout << "Runtime Error" << std::endl;
		exit(0);
	}

	if(true/*status ==*/ /*SubmissionProcess::Accepted*/)
	{
		QString outputFile = parameters.workingDirectory + "output.txt";
	
 		if(!QFile::exists(outputFile))
 		{
 			std::cout << "Test " << testNumber << ":\nNo output file" << std::endl;
 			exit(0);
 		}
		QString program = contestFolder + "tasks/" + taskNumber + ".exe";
		QStringList arguments;
		QProcess process;
		arguments << currentInputFile << outputFile << currentAnswerFile;
		process.start(program, arguments);
		process.waitForStarted();
		process.waitForFinished();

		const int exitCode = process.exitCode();

		if(exitCode == 0)
			processnextSubmit();
		else if(exitCode == 2)
		{
			std::cout << "Test "<< testNumber << ":\nPE" << std::endl;
			exit(0);
		}
		else /*if	(exitCode == 1)*/
		{
			std::cout << "Test "<< testNumber << ":\nWA" << std::endl;
			exit(0);
		}

	}
//	std::cout << "Test "<< testNumber << ":\nRuntime Error" << std::endl;
//	exit(0);
}
