#include "QChar.h"
#include <cctype>

QChar::QChar() : c('\0') {}
QChar::QChar(char ch) : c(ch) {}
QChar::QChar(const QChar& other) : c(other.c) {}  // Copy constructor

char QChar::toChar() const { return c; }
bool QChar::isLetter() const { return std::isalpha(c); }
bool QChar::isDigit() const { return std::isdigit(c); }
char QChar::toLatin1() const { return c; }

// Implicit conversion to char
QChar::operator char() const { return c; }

// Comparison operators
bool QChar::operator==(char other) const { return c == other; }
bool QChar::operator!=(char other) const { return c != other; }
bool QChar::operator<(char other) const { return c < other; }
bool QChar::operator>(char other) const { return c > other; }
bool QChar::operator<=(char other) const { return c <= other; }
bool QChar::operator>=(char other) const { return c >= other; }

// Comparison operators with QChar
bool QChar::operator==(const QChar& other) const { return c == other.c; }
bool QChar::operator!=(const QChar& other) const { return c != other.c; }
bool QChar::operator<(const QChar& other) const { return c < other.c; }
bool QChar::operator>(const QChar& other) const { return c > other.c; }
bool QChar::operator<=(const QChar& other) const { return c <= other.c; }
bool QChar::operator>=(const QChar& other) const { return c >= other.c; }

// Comparison operators with int
bool QChar::operator==(int other) const { return c == static_cast<char>(other); }
bool QChar::operator!=(int other) const { return c != static_cast<char>(other); }
bool QChar::operator<(int other) const { return c < static_cast<char>(other); }
bool QChar::operator>(int other) const { return c > static_cast<char>(other); }
bool QChar::operator<=(int other) const { return c <= static_cast<char>(other); }
bool QChar::operator>=(int other) const { return c >= static_cast<char>(other); }

