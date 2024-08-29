#ifndef PUTIL_H
#define PUTIL_H

//#include <QString>
#include "../qtext_simple/QString.h"

// additional method; replaces regex: QRegExp rx("(\\d+)");
bool containsDigits(QString &str) {
    for (const QChar& c : str) {
        if (c.isDigit()) {
            return true;
        }
    }
    return false;
}

#endif // PUTIL_H