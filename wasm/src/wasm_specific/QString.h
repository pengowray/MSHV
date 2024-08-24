#ifndef QSTRING_H
#define QSTRING_H

#include <string>
#include <vector>
#include "QChar.h"
#include "QStringList.h"

class QChar;

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
    char& operator[](int index);
    const char& operator[](int index) const;
    bool isLetter() const;
    bool isDigit() const;
    char toLatin1() const;
    QString& replace(const QString& before, const QString& after);
    QString midRef(int pos, int len = -1) const;
    QString rightJustified(int width, QChar fill = QChar(' ')) const;
    QString toUpper() const;
    bool isEmpty() const;

    // Forward declaration of QStringList
    class QStringList;
    QStringList split(const QString& separator) const;
};

#endif // QSTRING_H