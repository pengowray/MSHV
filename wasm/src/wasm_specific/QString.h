#ifndef QSTRING_H
#define QSTRING_H

#include <string>
#include <vector>
#include "QChar.h"

// Forward declaration of QStringList
class QStringList;

class QString : public std::string {
public:
    QString();
    QString(const char* str);
    QString(const std::string& str);
    QString(int size, QChar ch);

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
    int toInt() const;
    int toInt(bool* ok) const;
    int toInt(bool* ok, int base) const;
    QString arg(int n, int width = 0, int base = 10, QChar fill = QChar(' ')) const;
    QString& remove(QChar ch);
    QString& remove(const QString& str);
    QString& prepend(const QString& str);
    QString& prepend(char ch);
    QChar at(int index) const;
    //char& operator[](int index);
    //const char& operator[](int index) const;
    QChar operator[](int index);
    const QChar operator[](int index) const;
        
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

    QStringList split(const QString& separator) const;

    class const_iterator {
        std::string::const_iterator it;
    public:
        const_iterator(std::string::const_iterator i) : it(i) {}
        const_iterator& operator++() { ++it; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
        bool operator!=(const const_iterator& other) const { return it != other.it; }
        QChar operator*() const { return QChar(*it); }
    };

    iterator begin() { return iterator(std::string::begin()); }
    iterator end() { return iterator(std::string::end()); }
    const_iterator begin() const { return const_iterator(std::string::begin()); }
    const_iterator end() const { return const_iterator(std::string::end()); }
};

// macro for: foreach(QChar Char, callsign) {
// or instead use range-based for loop syntax: for (QChar Char : callsign) {
#define foreach(variable, container) \
    for (auto it = (container).begin(); it != (container).end(); ++it) \
        if (bool _foreach_flag = false) {} \
        else for (variable = *it; !_foreach_flag; _foreach_flag = true)

#endif // QSTRING_H