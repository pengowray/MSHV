#include <emscripten.h>
#include <string>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <vector>
#include "QString.h"
#include "QStringList.h"

// Minimal QString implementation, wrapping std::string

//class QStringList; // Forward declaration of QString

QString::QString() : std::string() {}
QString::QString(const char* str) : std::string(str) {}
QString::QString(const std::string& str) : std::string(str) {}
QString::QString(int size, QChar ch) : std::string(size, ch.toChar()) {}
QString::QString(const char* str, size_t length) : std::string(str, length) {}

template<class InputIt>
QString::QString(InputIt first, InputIt last) : std::string(first, last) {}

QString& QString::operator+=(const QString& str) {
    std::string::operator+=(str);
    return *this;
}

std::string QString::toStdString() const {
    return *this;
}

const char* QString::c_str() const {
    return std::string::c_str();
}

int QString::size() const {
    return std::string::size();
}

// Alias for size()
int QString::count() const {
    return size();
}

QString& QString::append(const QString& str) {
    std::string::append(str);
    return *this;
}

QString QString::mid(int pos, int len) const {
    if (pos >= (int)size()) return QString();
    if (len < 0) len = size() - pos;
    return QString(std::string::substr(pos, len));
}
    
int QString::indexOf(const QString& str, int from) const {
    size_t pos = std::string::find(str, from);
    return pos == std::string::npos ? -1 : static_cast<int>(pos);
}
int QString::indexOf(char ch, int from) const {
    size_t pos = std::string::find(ch, from);
    return pos == std::string::npos ? -1 : static_cast<int>(pos);
}

int QString::indexOf(QChar ch, int from) const {
    return indexOf(ch.toChar(), from);
}

bool QString::contains(const QString& str) const {
    return std::string::find(str) != std::string::npos;
}

QString QString::trimmed() const {
    const char* start = c_str();
    const char* end = start + size();
    while (start < end && std::isspace(*start)) ++start;
    while (end > start && std::isspace(*(end - 1))) --end;
    return QString(start, end - start);
}

int QString::toInt() const {
    return std::stoi(*this);
}

int QString::toInt(bool* ok) const {
    try {
        int result = std::stoi(*this);
        if (ok) *ok = true;
        return result;
    } catch (const std::exception&) {
        if (ok) *ok = false;
        return 0;
    }
}

int QString::toInt(bool* ok, int base) const {
    try {
        size_t pos;
        int result = std::stoi(*this, &pos, base);
        if (ok) *ok = (pos == this->length());
        return result;
    } catch (const std::exception&) {
        if (ok) *ok = false;
        return 0;
    }
}
/*
QString QString::arg(int n, int width, int base, QChar fill) const {
    std::ostringstream oss;
    oss << std::setw(width) << std::setfill(fill.toChar()) << std::setbase(base) << n;
    return QString(std::string(*this) + oss.str());
}
*/
QString QString::arg(const QString& a, int fieldWidth, QChar fillChar) const {
    return argToString(a, fieldWidth, fillChar);
}

QString QString::arg(int a, int fieldWidth, int base, QChar fillChar) const {
    return argToString(static_cast<long>(a), fieldWidth, base, fillChar);
}

QString QString::arg(long a, int fieldWidth, int base, QChar fillChar) const {
    return argToString(a, fieldWidth, base, fillChar);
}

QString QString::arg(double a, int fieldWidth, char format, int precision, QChar fillChar) const {
    return argToString(a, fieldWidth, format, precision, fillChar);
}

QString QString::argToString(const QString& a, int fieldWidth, QChar fillChar) const {
    QString result = *this;
    QString formatted = a.rightJustified(fieldWidth, fillChar);
    size_t pos = result.find("%1");
    if (pos != std::string::npos) {
        result.replace(pos, 2, formatted);
    }
    return result;
}

QString QString::argToString(long a, int fieldWidth, int base, QChar fillChar) const {
    QString result = *this;
    std::ostringstream oss;
    oss << std::setw(fieldWidth) << std::setfill(fillChar.toChar()) 
        << std::setbase(base) << a;
    QString formatted = oss.str();
    size_t pos = result.find("%1");
    if (pos != std::string::npos) {
        result.replace(pos, 2, formatted);
    }
    return result;
}

QString QString::argToString(double a, int fieldWidth, char format, int precision, QChar fillChar) const {
    QString result = *this;
    std::ostringstream oss;
    oss << std::setw(fieldWidth) << std::setfill(fillChar.toChar()) 
        << std::setprecision(precision) << std::fixed;
    if (format == 'g' || format == 'G')
        oss << std::defaultfloat;
    else if (format == 'f' || format == 'F')
        oss << std::fixed;
    else if (format == 'e' || format == 'E')
        oss << std::scientific;
    oss << a;
    QString formatted = oss.str();
    size_t pos = result.find("%1");
    if (pos != std::string::npos) {
        result.replace(pos, 2, formatted);
    }
    return result;
}

QString QString::arg(const char* fmt, ...) const {
    va_list ap;
    va_start(ap, fmt);
    QString result = varg(*this, ap);
    va_end(ap);
    return result;
}

QString QString::varg(const QString& format, va_list ap) {
    std::vector<char> buf(1024);
    while (true) {
        va_list ap_copy;
        va_copy(ap_copy, ap);
        int result = std::vsnprintf(buf.data(), buf.size(), format.c_str(), ap_copy);
        va_end(ap_copy);
        
        if (result < 0) {
            return QString();
        }
        
        if (result < static_cast<int>(buf.size())) {
            return QString(buf.data());
        }
        
        buf.resize(result + 1);
    }
}

QString& QString::remove(QChar ch) {
    auto newEnd = std::remove(std::string::begin(), std::string::end(), ch.toChar());
    std::string::erase(newEnd, std::string::end());
    return *this;
}

// Remove all occurrences of a string
QString& QString::remove(const QString& str) {
    size_t pos = 0;
    while ((pos = this->find(str, pos)) != std::string::npos) {
        std::string::erase(pos, str.length());
    }
    return *this;
}

// Prepend a string to the beginning
QString& QString::prepend(const QString& str) {
    this->insert(0, str);
    return *this;
}

// Prepend a single character
QString& QString::prepend(char ch) {
    this->insert(0, 1, ch);
    return *this;
}

// Append a single character
QString& QString::append(char ch) {
    std::string::push_back(ch);
    return *this;
}

QChar QString::at(int index) const {
    return QChar(std::string::at(index));
}

//char& operator[](int index) { return std::string::operator[](index); }
//const char& operator[](int index) const { return std::string::operator[](index); }

QChar QString::operator[](int index) { return QChar(std::string::at(index)); }
const QChar QString::operator[](int index) const { return QChar(std::string::at(index)); }


bool QString::isLetter() const {
    return !empty() && std::isalpha(front());
}

bool QString::isDigit() const {
    return !empty() && std::isdigit(front());
}

// toLatin1 method (for single character strings)
char QString::toLatin1() const {
    return empty() ? '\0' : front();
}

QString& QString::replace(const QString& before, const QString& after) {
    size_t start_pos = 0;
    while((start_pos = find(before, start_pos)) != std::string::npos) {
        std::string::replace(start_pos, before.length(), after);
        start_pos += after.length();
    }
    return *this;
}

QString& QString::replace(int start, int count, const QString& replacement) {
    // Ensure the start position and count are within valid bounds
    if (start < 0 || start >= static_cast<int>(size()) || count < 0 || start + count > static_cast<int>(size())) {
        throw std::out_of_range("Index out of range");
    }

    // Replace the specified range with the replacement string
    std::string::replace(start, count, replacement);

    return *this;
}

QString QString::midRef(int pos, int len) const {
    return mid(pos, len);
}

QString QString::leftJustified(int width, QChar fill) const {
    if (static_cast<int>(size()) >= width) {
        return *this;
    }
    return QString(*this + QString(width - size(), fill));
}

QString QString::rightJustified(int width, QChar fill) const {
    if (static_cast<int>(size()) >= width) {
        return *this;
    }
    return QString(width - size(), fill) + *this;
}

QString QString::toUpper() const {
    QString result(*this);
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

bool QString::isEmpty() const {
    return empty();
}

// additional method (not from QString); replaces regex: QRegExp rx("(\\d+)");
// TODO: move to utility functions file
bool QString::containsDigits() const {
    for (char c : *this) {
        if (std::isdigit(c)) {
            return true;
        }
    }
    return false;
}

QStringList QString::split(const QString& separator) const {
    QStringList result;
    size_t start = 0;
    size_t end = find(separator);
    while (end != std::string::npos) {
        result.push_back(substr(start, end - start));
        start = end + separator.length();
        end = find(separator, start);
    }
    result.push_back(substr(start));
    return result;
}

// QString::iterator implementations
QString::iterator::iterator(std::string::iterator i) : it(i) {}
QString::iterator& QString::iterator::operator++() { ++it; return *this; }
QString::iterator QString::iterator::operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
bool QString::iterator::operator!=(const iterator& other) const { return it != other.it; }
QChar QString::iterator::operator*() const { return QChar(*it); }

// QString::const_iterator implementations
QString::const_iterator::const_iterator(std::string::const_iterator i) : it(i) {}
QString::const_iterator& QString::const_iterator::operator++() { ++it; return *this; }
QString::const_iterator QString::const_iterator::operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
bool QString::const_iterator::operator!=(const const_iterator& other) const { return it != other.it; }
QChar QString::const_iterator::operator*() const { return QChar(*it); }

// QString iterator methods
QString::iterator QString::begin() { return iterator(std::string::begin()); }
QString::iterator QString::end() { return iterator(std::string::end()); }
QString::const_iterator QString::begin() const { return const_iterator(std::string::begin()); }
QString::const_iterator QString::end() const { return const_iterator(std::string::end()); }

// Free functions
//QString operator+(const QString& qstr, const std::string& stdStr) { return QString(qstr.toStdString() + stdStr); }
QString operator+(const QString& qstr, const std::string& stdStr) { return QString(std::string(qstr) + stdStr); }
QString operator+(const QString& qstr, const char* cstr) { return QString(qstr.toStdString() + cstr); }
QString operator+(const QString& qstr1, const QString& qstr2) { return QString(std::string(qstr1) + std::string(qstr2)); }
QString operator+(const char* cstr, const QString& qstr) { return QString(std::string(cstr) + std::string(qstr)); }