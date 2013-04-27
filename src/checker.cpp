#include "checker.h"
#include "util.h"

Checker::Checker(QObject *parent, const ProcessParameters &parameters, QStringList & inputs, QStringList & answers, const QString & contestFolder, QString taskNumber)
    : QObject(parent), parameters(parameters), tests(inputs, answers), process(parameters), contestFolder(contestFolder), taskNumber(taskNumber)
{
    connect(&process, SIGNAL(finished(int)), SLOT(checkSubmitStatus(int)));
    testNumber = 0;
}

void Checker::startChecking()
{
    processNextSubmission();
}

void Checker::processNextSubmission()
{
    if(tests.empty())
    {
        printToStdout("AC\n");
        qApp->exit(0);
        return;
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
        printToStdout(QString("Test %1:\nMemory Limit\n").arg(testNumber));
        qApp->exit(0);
    }
    else if(status == SubmissionProcess::TimeLimitError)
    {
        printToStdout(QString("Test %1:\nTime Limit\n").arg(testNumber));
        qApp->exit(0);
    }
    else if(status == SubmissionProcess::ProgramStartError)
    {
        printToStdout("Error\n");
        qApp->exit(0);
    }
    else if(status == SubmissionProcess::RunTimeError)
    {
        printToStdout(QString("Test %1:\nRuntime Error\n").arg(testNumber));
        qApp->exit(0);
    }
    else {
        QString outputFile = parameters.workingDirectory + "output.txt";
        if(!QFile::exists(outputFile)) {
            printToStdout(QString("Test %1:\nNo output file").arg(testNumber));
            qApp->exit(0);
            return;
        }

        QString program = contestFolder + "tasks/" + taskNumber + ".exe";
        QStringList arguments;
        QProcess process;
        arguments << currentInputFile << outputFile << currentAnswerFile;
        process.start(program, arguments);
        process.waitForStarted();
        process.waitForFinished();

        const int exitCode = process.exitCode();

        if(exitCode == 0) {
            processNextSubmission();
        }
        else if(exitCode == 2)
        {
            printToStdout(QString("Test %1:\nPE\n").arg(testNumber));
            qApp->exit(0);
        }
        else
        {
            printToStdout(QString("Test %1:\nWA\n").arg(testNumber));
            qApp->exit(0);
        }

    }
}
