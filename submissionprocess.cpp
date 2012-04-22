#include "submissionprocess.h"
#include "submission.h"
#include <iostream>

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#include <QDebug>

SubmissionProcess::SubmissionProcess(const ProcessParameters & parameters, QObject *parent)
	: QObject(parent),
	parameters(parameters),
	timeLapsed(0)
{
	connect(&timer, SIGNAL(timeout()), SLOT(checkProcess()));
}

SubmissionProcess::~SubmissionProcess()
{
}

void SubmissionProcess::start()
{
	STARTUPINFO si = {};
	si.cb = sizeof(si);
	timeLapsed = 0;

	bool result = ::CreateProcessWithLogonW(L"Администратор", 
		L".", 
		L"cooler", 
		LOGON_NETCREDENTIALS_ONLY, 
		(WCHAR *) parameters.programPath.constData(), 
		NULL, 
		NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_DEFAULT_ERROR_MODE, 
		NULL, 
		(WCHAR *) parameters.workingDirectory.constData(),
		&si,
		&processInformation);



	if(!result)
	{
		std::cout << "Failed to create process. Error code - " << GetLastError();
		emit finished(ProgramStartError);
	}

	timer.start(Timeout);
}

QString SubmissionProcess::errorText()
{
	return error;
}

void SubmissionProcess::checkProcess()
{
	timeLapsed += Timeout;
	
	const int status = getProcessStatus();
 	
	if(status != Active)
	{
		processFinished(status);
		return;
	}

	const int memorySize = getProcessMemorySize();
	if(memorySize > parameters.memoryLimit)
		terminateProcessWithRuntimeError(MemoryLimit);

	int kernelTime = getProcessRunninglTime();
	if(kernelTime > parameters.timeLimit || timeLapsed > parameters.timeLimit + 5000)
		terminateProcessWithRuntimeError(TimeLimitError);
}

int SubmissionProcess::getProcessMemorySize()
{
	PROCESS_MEMORY_COUNTERS pmc;
	bool r = ::GetProcessMemoryInfo(processInformation.hProcess, &pmc, sizeof(pmc));
	return r ? pmc.WorkingSetSize : 0;
}

int SubmissionProcess::getProcessStatus()
{
	DWORD status = 0;
	::GetExitCodeProcess(processInformation.hProcess, &status);
	if(status == STILL_ACTIVE)
		return Active;
	else if(status == 0)
		return Finished;
	else
		return FailFinish;
}

int SubmissionProcess::getProcessRunninglTime()
{
	FILETIME creationTime, exitTime, kernelTime, userTime;
	::GetProcessTimes(processInformation.hProcess, &creationTime, &exitTime, &kernelTime, &userTime);

	__int64 time1 = *(__int64 *)(&kernelTime)  / 1000 / 10;
	__int64 time2 =  *(__int64 *)(&userTime) / 1000 / 10;
	// result can overflow but well since we usually need to tun the process for like 2 seconds there is no way it's gonna happen in practice
	return (int) time1 + time2;
}

void SubmissionProcess::terminateProcessWithRuntimeError(const int limitType)
{
	::TerminateProcess(processInformation.hProcess, 0);
	timer.stop();
	timeLapsed = 0;
	emit finished(limitType);
}

void SubmissionProcess::processFinished(int exitState)
{
	timer.stop();
	timeLapsed = 0;
	if(!exitState)
		emit finished(Accepted);
	emit finished(WrongExitCode);
}