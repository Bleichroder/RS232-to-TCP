#ifndef UTIL_H
#define UTIL_H

#include <QFile>
#include <QTextCodec>

class Util
{
public:
    Util();
    ~Util();

public:
    static QFile *file;
    static int des;
};

#endif // UTIL_H
