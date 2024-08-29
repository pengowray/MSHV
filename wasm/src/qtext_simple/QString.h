#ifndef QSTRING_H
#define QSTRING_H

#include <string>
#include <cstdarg>
#include <vector>
#include "QChar.h"

class QStringList;

namespace Qt {
    enum SplitBehavior {
        KeepEmptyParts = 0,
        SkipEmptyParts = 1
    };
}

class QString : public std::string {
public:
    QString();
    QString(const char* str);
    QString(const std::string& str);
    QString(int size, QChar ch);
    QString(const char* str, size_t length);

    template<class InputIt>
    QString(InputIt first, InputIt last);

    QString& operator+=(const QString& str);
    std::string toStdString() const;
    const char* c_str() const;
    int size() const;
    int count() const;
    QString& append(const QString& str);
    QString& append(char ch);
    QString mid(int pos, int len = -1) const;
    int indexOf(const QString& str, int from = 0) const;
    int indexOf(char ch, int from = 0) const;
    int indexOf(QChar ch, int from = 0) const;
    bool contains(const QString& str) const;
    QString trimmed() const;
    //int toInt() const;
    //int toInt(bool* ok) const;
    int toInt(bool *ok = nullptr, int base = 10) const;
    //QString arg(int n, int width = 0, int base = 10, QChar fill = QChar(' ')) const;
    QString arg(const QString& a, int fieldWidth = 0, QChar fillChar = QChar(' ')) const;
    QString arg(int a, int fieldWidth = 0, int base = 10, QChar fillChar = QChar(' ')) const;
    QString arg(long a, int fieldWidth = 0, int base = 10, QChar fillChar = QChar(' ')) const;
    QString arg(double a, int fieldWidth = 0, char format = 'g', int precision = -1, QChar fillChar = QChar(' ')) const;
    QString arg(const char* fmt, ...) const;
    QString& remove(QChar ch);
    QString& remove(const QString& str);
    QString& prepend(const QString& str);
    QString& prepend(char ch);
    QChar at(int index) const;
    QChar& operator[](int index);
    const QChar& operator[](int index) const;
            
    bool isLetter() const;
    bool isDigit() const;
    char toLatin1() const;
    QString& replace(const QString& before, const QString& after);
    QString& replace(int start, int count, const QString& replacement);
    QString midRef(int pos, int len = -1) const;
    QString leftJustified(int width, QChar fill = QChar(' ')) const;
    QString rightJustified(int width, QChar fill = QChar(' ')) const;
    QString toUpper() const;
    bool isEmpty() const;
    bool containsDigits() const;
    QStringList split(const QString& separator, Qt::SplitBehavior behavior) const;
    QStringList split(QChar sep, Qt::SplitBehavior behavior) const;
    QStringList split(QChar sep) const;
    QStringList split(const QString& separator) const;  // Qt::SplitBehavior behavior = Qt::KeepEmptyParts (default)
    QStringList split_skip_empty(const QString& separator) const; // Qt::SkipEmptyParts 
    QString left(int n) const;

    QString& erase(size_type pos = 0, size_type count = npos) {
        std::string::erase(pos, count);
        return *this;
    }

    QString& erase(const_iterator position) {
        std::string::erase(position);
        return *this;
    }

    QString& erase(const_iterator first, const_iterator last) {
        std::string::erase(first, last);
        return *this;
    }

    class iterator {
        std::string::iterator it;
    public:
        iterator(std::string::iterator i);
        iterator& operator++();
        iterator operator++(int);
        bool operator!=(const iterator& other) const;
        QChar operator*() const;
        std::string::iterator base() { return it; }
    };

    class const_iterator {
        std::string::const_iterator it;
    public:
        const_iterator(std::string::const_iterator i);
        const_iterator& operator++();
        const_iterator operator++(int);
        bool operator!=(const const_iterator& other) const;
        QChar operator*() const;
        std::string::const_iterator base() const { return it; } 
    };

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    private:
        QString argToString(const QString& a, int fieldWidth, QChar fillChar) const;
        QString argToString(long a, int fieldWidth, int base, QChar fillChar) const;
        QString argToString(double a, int fieldWidth, char format, int precision, QChar fillChar) const;
        static QString varg(const QString& format, va_list ap);
};

QString operator+(const QString& qstr1, const QString& qstr2);
QString operator+(const QString& qstr, const std::string& stdStr);
QString operator+(const QString& qstr, const char* cstr);
QString operator+(const char* cstr, const QString& qstr);

// macro for: foreach(QChar Char, callsign) {
// or instead use range-based for loop syntax: for (QChar Char : callsign) {
#define foreach(variable, container) \
    for (auto it = (container).begin(); it != (container).end(); ++it) \
        if (bool _foreach_flag = false) {} \
        else for (variable = *it; !_foreach_flag; _foreach_flag = true)

#endif // QSTRING_H
