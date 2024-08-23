#ifndef QSTRING_H
#define QSTRING_H

#include <string>

class QString : public std::string {
public:
    QString() : std::string() {}
    QString(const char* str) : std::string(str) {}
    QString(const std::string& str) : std::string(str) {}

    int toInt() const { return std::stoi(*this); }
    float toFloat() const { return std::stof(*this); }
    double toDouble() const { return std::stod(*this); }

    QString& append(const QString& str) {
        std::string::append(str);
        return *this;
    }

    QString mid(int pos, int len = -1) const {
        return QString(std::string::substr(pos, len));
    }

    int indexOf(const QString& str, int from = 0) const {
        return std::string::find(str, from);
    }

    bool contains(const QString& str) const {
        return std::string::find(str) != std::string::npos;
    }

    QString trimmed() const {
        auto wsfront = std::find_if_not(this->begin(), this->end(), [](int c){return std::isspace(c);});
        auto wsback = std::find_if_not(this->rbegin(), this->rend(), [](int c){return std::isspace(c);}).base();
        return (wsback <= wsfront ? QString() : QString(wsfront, wsback));
    }
};

#endif // QSTRING_H