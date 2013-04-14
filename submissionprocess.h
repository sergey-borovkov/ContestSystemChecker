#ifndef SUBMISSIONPROCESS_H
#define SUBMISSIONPROCESS_H

#include <QObject>
#include <QTimer>
#include <QString>

#define NOMINMAX
#include <windows.h>

struct ProcessParameters
{
    QString programPath;
    QString workingDirectory;
    QString language;
    int timeLimit;
    int memoryLimit;
};

class SubmissionProcess : public QObject
{
    Q_OBJECT
public:
    enum SubmissionVerdict
    {
        Accepted,
        WrongAnswer,
        RunTimeError,
        MemoryLimit,
        PresenationError,
        NoOutput,
        TimeLimitError,
        CompileError,
        ProgramStartError,
        WrongExitCode
    };

    SubmissionProcess(const ProcessParameters & parameters, QObject *parent = 0);
    ~SubmissionProcess();
    void start();
    QString errorText();
    int timeLapsed;

signals:
    void finished(int verdict);
private slots:
    void checkProcess();
    void startProcess();
    void runtimeError();
private:
    enum {Active, Finished, FailFinish};

    int getProcessMemorySize();
    int getProcessRunninglTime();
    int getProcessStatus();
    void terminateProcessWithRuntimeError(const int limitType);
    void processFinished(int exitState);

    enum {Timeout = 100};
    PROCESS_INFORMATION processInformation;
    ProcessParameters parameters;
    QString error;
    QTimer timer;
};

#endif // SUBMISSIONPROCESS_H
