#ifndef QSTRINGLIST_H
#define QSTRINGLIST_H

#include <vector>
#include "QString.h"

class QStringList : public std::vector<QString> {
public:
    QStringList();
    QStringList(const std::initializer_list<QString>& list);

    void append(const QString& str);
    QString join(const QString& separator) const;
    int count() const;
};

#endif // QSTRINGLIST_H