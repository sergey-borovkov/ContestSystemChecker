#include "debugger.h"
#include "util.h"

#include <QDebug>
#include <QCoreApplication>

Debugger::Debugger(ProcessParameters parameters, PROCESS_INFORMATION &processInformation, QObject *parent)
    : QThread(parent), parameters(parameters), processInformation(processInformation)
{

}

void Debugger::run()
{
    STARTUPINFO si = {};
    si.cb = sizeof(si);
    DWORD creationFlags = NORMAL_PRIORITY_CLASS | CREATE_DEFAULT_ERROR_MODE;
    if(parameters.language != QLatin1String("c_sharp"))
        creationFlags |=  DEBUG_ONLY_THIS_PROCESS;

    qDebug() << parameters.programPath;
    bool result = ::CreateProcess((WCHAR *) parameters.programPath.constData(),
                                  NULL, NULL, NULL, FALSE,
                                  creationFlags,
                                  NULL,
                                  (WCHAR *) parameters.workingDirectory.constData(),
                                  &si,
                                  &processInformation);

    if(!result)
    {
        qDebug() <<"Failed to create process. Error code - " << GetLastError();
        qApp->exit(-1);
    }

    /* fucking managed code does not like to be debugged */
    if (parameters.language == QLatin1String("c_sharp")) {
        emit ready();
        exit(0);
    }

    ::DebugActiveProcess(processInformation.dwProcessId);
    emit ready();
    DEBUG_EVENT event;

    while (WaitForDebugEvent(&event, INFINITE)) {
        qDebug() << event.dwDebugEventCode;
        DWORD dwStatus = DBG_CONTINUE;
        if (event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
        {
            dwStatus = DBG_EXCEPTION_NOT_HANDLED;
            qDebug() << event.u.Exception.ExceptionRecord.ExceptionCode;
            if( event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_ACCESS_VIOLATION ||
                    event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_ARRAY_BOUNDS_EXCEEDED ||
                    event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_FLT_STACK_CHECK ||
                    event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_ILLEGAL_INSTRUCTION ||
                    event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO ||
                    event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_NONCONTINUABLE_EXCEPTION ||
                    event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_STACK_OVERFLOW ||
                    event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_PRIV_INSTRUCTION) {
                emit runtimeError();
                exit(0);
            }
        }
        else if(event.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
        {
            qDebug() << "Debugger exit...";
            qApp->exit(0);
        }

        ContinueDebugEvent(event.dwProcessId, event.dwThreadId, dwStatus);
    }
}
