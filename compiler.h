#ifndef COMPILER_H
#define COMPILER_H

#include <QObject>

class QString;

class Compiler
{
public:
	Compiler();
	virtual ~Compiler();
	virtual bool compile(const QString & path, const QString & pathToCompiledProgram) = 0;
};

class CppCompiler : public Compiler
{
public:
	CppCompiler() {}
	~CppCompiler() {}
	virtual bool compile(const QString & path, const QString & pathToCompiledProgram);
};

class DelphiCompiler : public Compiler
{
public:
	DelphiCompiler() {}
	~DelphiCompiler() {}
	virtual bool compile(const QString & path, const QString & pathToCompiledProgram);
};

class CSharpCompiler: public Compiler
{
public:
	CSharpCompiler() {}
	~CSharpCompiler() {}
	virtual bool compile(const QString & path, const QString & pathToCompiledProgram);
};

Compiler *makeCompiler(const QString & languageName);

#endif // COMPILER_H
