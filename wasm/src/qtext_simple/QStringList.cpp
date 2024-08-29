#include "QStringList.h"
#include "QString.h"

// Forward declaration of QString
//class QString;

QStringList::QStringList() : std::vector<QString>() {}
QStringList::QStringList(const std::initializer_list<QString>& list) : std::vector<QString>(list) {}

void QStringList::append(const QString& str) {
    this->push_back(str);
}

QString QStringList::join(const QString& separator) const {
    QString result;
    for (size_t i = 0; i < this->size(); ++i) {
        if (i > 0) result += separator;
        result += this->at(i);
    }
    return result;
}

int QStringList::count() const {
    return static_cast<int>(this->size());
}

QStringList& QStringList::operator<<(const QString& str) {
    this->push_back(str);
    return *this;
}