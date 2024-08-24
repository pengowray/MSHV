#include "QChar.h"

class QChar {
private:
    char c;

public:
    QChar(char ch) : c(ch) {}
    QChar(const QChar& other) : c(other.c) {}  // Copy constructor
    
    char toChar() const { return c; }
    bool isLetter() const { return std::isalpha(c); }
    bool isDigit() const { return std::isdigit(c); }
    char toLatin1() const { return c; }

    // Implicit conversion to char
    operator char() const { return c; }

    // Comparison operators
    bool operator==(char other) const { return c == other; }
    bool operator!=(char other) const { return c != other; }
    bool operator<(char other) const { return c < other; }
    bool operator>(char other) const { return c > other; }
    bool operator<=(char other) const { return c <= other; }
    bool operator>=(char other) const { return c >= other; }
    bool operator<=(int other) const { return c <= static_cast<char>(other); }
    bool operator>=(int other) const { return c >= static_cast<char>(other); }

    // Comparison operators with QChar
    bool operator==(const QChar& other) const { return c == other.c; }
    bool operator!=(const QChar& other) const { return c != other.c; }
    bool operator<(const QChar& other) const { return c < other.c; }
    bool operator>(const QChar& other) const { return c > other.c; }
    bool operator<=(const QChar& other) const { return c <= other.c; }
    bool operator>=(const QChar& other) const { return c >= other.c; }
};