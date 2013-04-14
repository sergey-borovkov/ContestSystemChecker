#include "compiler.h"

#include <QProcess>
#include <QDebug>

bool CppCompiler::compile( const QString & path, const QString & pathToCompiledProgram)
{
    QString program = "C:/Program Files (x86)/Microsoft Visual Studio 10.0/VC/bin/cl.exe";
    QStringList arguments;
    arguments << path;
    arguments << "/W4" << "/F268435456" << "/EHsc" << "/O2" << "/DONLINE_JUDGE" << path << "-o" << pathToCompiledProgram;

    QProcess process;

    //	process.setWorkingDirectory(pathToCompiledProgram.mid(0, pathToCompiledProgram.lastIndexOf('/')));
    process.start(program, arguments);
    process.waitForStarted();
    process.waitForFinished();
    QByteArray a = process.readAllStandardOutput();
    const int exitCode = process.exitCode();
    return !exitCode;
}

QSharedPointer<Compiler> makeCompiler(const QString &languageName)
{
    if(languageName == "cpp")
        return QSharedPointer<Compiler>(new CppCompiler);
    else if (languageName == "delphi")
        return QSharedPointer<Compiler>(new DelphiCompiler);
    else if(languageName == "c_sharp")
        return QSharedPointer<Compiler>(new CSharpCompiler);
    else
        return QSharedPointer<Compiler>();
}

bool DelphiCompiler::compile( const QString & path, const QString & pathToCompiledProgram )
{
    QString program = "dcc32";
    QStringList arguments;
    QString tmp = path;
    tmp.replace("/", "\\");
    QString tmp2 = pathToCompiledProgram.mid(0, pathToCompiledProgram.lastIndexOf('/'));
    tmp2.replace("/", "\\");
    arguments << tmp << "-E" + tmp2;

    QProcess process;
    process.setWorkingDirectory(pathToCompiledProgram.mid(0, pathToCompiledProgram.lastIndexOf('/')));
    process.start(program, arguments);
    process.waitForStarted();
    process.waitForFinished();
    QByteArray a = process.readAllStandardOutput();
    const int exitCode = process.exitCode();
    return !exitCode;
}

bool CSharpCompiler::compile( const QString & path, const QString & pathToCompiledProgram )
{
    QString program = "C:/Windows/Microsoft.NET/Framework/v2.0.50727/csc.exe";
    QStringList arguments;
    QString tmp = path;
    QString tmp2 = pathToCompiledProgram.mid(0, pathToCompiledProgram.lastIndexOf('/'));
    tmp2.replace("/", "\\");
    tmp.replace("/", "\\");
    arguments << QString("/out:%1").arg(pathToCompiledProgram) << tmp;

    QProcess process;
    process.setWorkingDirectory(pathToCompiledProgram.mid(0, pathToCompiledProgram.lastIndexOf('/')));
    process.start(program, arguments);
    process.waitForStarted();
    process.waitForFinished();
    QByteArray a = process.readAllStandardOutput();
    const int exitCode = process.exitCode();
    return !exitCode;
}
