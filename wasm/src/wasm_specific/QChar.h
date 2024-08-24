#ifndef QCHAR_H
#define QCHAR_H

#include <cctype>

class QChar {
private:
    char c;

public:
    QChar();
    QChar(char ch);
    QChar(const QChar& other);
    
    char toChar() const;
    bool isLetter() const;
    bool isDigit() const;
    char toLatin1() const;

    operator char() const;

    bool operator==(char other) const;
    bool operator!=(char other) const;
    bool operator<(char other) const;
    bool operator>(char other) const;
    bool operator<=(char other) const;
    bool operator>=(char other) const;

    bool operator==(const QChar& other) const;
    bool operator!=(const QChar& other) const;
    bool operator<(const QChar& other) const;
    bool operator>(const QChar& other) const;
    bool operator<=(const QChar& other) const;
    bool operator>=(const QChar& other) const;

    bool operator==(int other) const;
    bool operator!=(int other) const;
    bool operator<(int other) const;
    bool operator>(int other) const;
    bool operator<=(int other) const;
    bool operator>=(int other) const;

};

#endif // QCHAR_H