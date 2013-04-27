#include <QtCore>
#include <QDebug>
#include <QtGlobal>

#include "submissionprocess.h"
#include "compiler.h"
#include "checker.h"
#include "util.h"


void noMessageOutput(QtMsgType, const QMessageLogContext&, const QString &)
{

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc != 5)
    {
        qDebug() << "Syntax: checker submission_id task_number language_name path_to_source";
        return 1;
    }

    qInstallMessageHandler(noMessageOutput);

    int submissionId = QString(argv[1]).toInt();
    QString taskNumber = QString(argv[2]);
    QString languageName(argv[3]);
    QString pathToSource(argv[4]);

    QString contestFolder = "C:/Disk_H/";
    QString pathToSubmissionFolder = contestFolder + "submissions/" + QString::number(submissionId) + '/';
    QDir testDirectory;
    if(!testDirectory.mkpath(pathToSubmissionFolder))
    {
        printToStdout("Unable to create folder " + pathToSubmissionFolder + "\n");
        return 1;
    }

    QSharedPointer<Compiler> c = makeCompiler(languageName);
    if(c.isNull())
    {
        printToStdout(QString("Compiling %1 is not supported\n").arg(languageName));
        return 1;
    }

    QString pathToProgram = pathToSubmissionFolder + QString::number(submissionId) + ".exe";

    if(!c->compile(pathToSource, pathToProgram))
    {
        printToStdout("Compile error\n");
        return 0;
    }

    testDirectory.cd(pathToSubmissionFolder);

    testDirectory.cd(QString("../../tasks/%1").arg(taskNumber));
    testDirectory.setSorting(QDir::Name);

    QStringList tests = testDirectory.entryList(QStringList() << "*.in");
    QStringList answers = testDirectory.entryList(QStringList() << "*.ans");

    if(tests.size() != answers.size())
    {
        printToStdout("Number of tests must be equal number of output files");
        return 1;
    }

    for(int i = 0; i < tests.size(); i++)
    {
        tests[i] = testDirectory.absoluteFilePath(tests[i]);
        answers[i] = testDirectory.absoluteFilePath(answers[i]);
    }

    ProcessParameters p;
    p.workingDirectory = pathToSubmissionFolder;
    p.programPath = pathToProgram;
    p.memoryLimit = 1024 * 1024 * 256;
    p.timeLimit = 2000;
    p.language = languageName;

    Checker checker(NULL, p, tests, answers, contestFolder, taskNumber);
    checker.startChecking();

    return a.exec();
}
