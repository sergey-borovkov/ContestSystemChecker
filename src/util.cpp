#include "util.h"

#include <QTextStream>

void printToStdout(const QString &string)
{
    QTextStream out(stdout, QIODevice::WriteOnly);
    out << string;
    out.flush();
}
