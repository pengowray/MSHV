#include "QStringList.h"

// Forward declaration of QString
class QString;

class QStringList : public std::vector<QString> {
public:
    QStringList() : std::vector<QString>() {}
    QStringList(const std::initializer_list<QString>& list) : std::vector<QString>(list) {}

    void append(const QString& str) {
        this->push_back(str);
    }

    QString join(const QString& separator) const {
        QString result;
        for (size_t i = 0; i < this->size(); ++i) {
            if (i > 0) result += separator;
            result += this->at(i);
        }
        return result;
    }

    int count() const {
        return static_cast<int>(this->size());
    }

    QStringList& operator<<(const QString& str) {
        this->push_back(str);
        return *this;
    }
};

QString operator+(const QString& qstr, const std::string& stdStr);
QString operator+(const QString& qstr, const char* cstr);
