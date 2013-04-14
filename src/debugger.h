#ifndef DEBUGGER_H
#define DEBUGGER_H

#define NOMINMAX

#include <QThread>
#include <Windows.h>
#include "submissionprocess.h"

class Debugger : public QThread
{
    Q_OBJECT
public:
    Debugger(ProcessParameters parameters, PROCESS_INFORMATION &processInformation, QObject *parent);

    void run();

signals:
    void ready();
    void runtimeError();
private:
    ProcessParameters parameters;
    PROCESS_INFORMATION &processInformation;
};

#endif // DEBUGGER_H
