#include <emscripten.h>
#include <string>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "QString.h"

class QString : public std::string {
public:
    QString() : std::string() {}
    QString(const char* str) : std::string(str) {}
    QString(const std::string& str) : std::string(str) {}
    QString(int size, QChar ch) : std::string(size, ch.toChar()) {}
    
    template<class InputIt>
    QString(InputIt first, InputIt last) : std::string(first, last) {}

    QString& operator+=(const QString& str) {
        std::string::operator+=(str);
        return *this;
    }

    std::string toStdString() const {
        return *this;
    }

    const char* c_str() const {
        return std::string::c_str();
    }
    
    int size() const {
        return std::string::size();
    }

    // Alias for size()
    int count() const {
        return size();
    }

    QString& append(const QString& str) {
        std::string::append(str);
        return *this;
    }
    
    QString mid(int pos, int len = -1) const {
        if (pos >= (int)size()) return QString();
        if (len < 0) len = size() - pos;
        return QString(std::string::substr(pos, len));
    }
        
    int indexOf(const QString& str, int from = 0) const {
        size_t pos = std::string::find(str, from);
        return pos == std::string::npos ? -1 : static_cast<int>(pos);
    }
    int indexOf(char ch, int from = 0) const {
        size_t pos = std::string::find(ch, from);
        return pos == std::string::npos ? -1 : static_cast<int>(pos);
    }

    int indexOf(QChar ch, int from = 0) const {
        return indexOf(ch.toChar(), from);
    }

    bool contains(const QString& str) const {
        return std::string::find(str) != std::string::npos;
    }
    
    QString trimmed() const {
        auto wsfront = std::find_if_not(this->begin(), this->end(), [](int c){return std::isspace(c);});
        auto wsback = std::find_if_not(this->rbegin(), this->rend(), [](int c){return std::isspace(c);}).base();
        return (wsback <= wsfront ? QString() : QString(wsfront, wsback));
    }

    int toInt() const {
        return std::stoi(*this);
    }

    int toInt(bool* ok) const {
        try {
            int result = std::stoi(*this);
            if (ok) *ok = true;
            return result;
        } catch (const std::exception&) {
            if (ok) *ok = false;
            return 0;
        }
    }

    int toInt(bool* ok, int base) const {
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

    QString arg(int n, int width = 0, int base = 10, QChar fill = QChar(' ')) const {
        std::ostringstream oss;
        oss << std::setw(width) << std::setfill(fill.toChar()) << std::setbase(base) << n;
        return *this + oss.str();
    }

    // Remove all occurrences of a character
    QString& remove(QChar ch) {
        this->erase(std::remove(this->begin(), this->end(), ch.toChar()), this->end());
        return *this;
    }

    // Remove all occurrences of a string
    QString& remove(const QString& str) {
        size_t pos = 0;
        while ((pos = this->find(str, pos)) != std::string::npos) {
            this->erase(pos, str.length());
        }
        return *this;
    }

    // Prepend a string to the beginning
    QString& prepend(const QString& str) {
        this->insert(0, str);
        return *this;
    }

    // Prepend a single character
    QString& prepend(char ch) {
        this->insert(0, 1, ch);
        return *this;
    }

    // Append a single character
    QString& append(char ch) {
        std::string::push_back(ch);
        return *this;
    }

    QChar at(int index) const {
        return QChar(std::string::at(index));
    }

    // Overload operator[] to return a reference to a character
    char& operator[](int index) {
        return std::string::operator[](index);
    }

    // Const version of operator[]
    const char& operator[](int index) const {
        return std::string::operator[](index);
    }

    bool isLetter() const {
        return !empty() && std::isalpha(front());
    }

    bool isDigit() const {
        return !empty() && std::isdigit(front());
    }

    // toLatin1 method (for single character strings)
    char toLatin1() const {
        return empty() ? '\0' : front();
    }

    QString& replace(const QString& before, const QString& after) {
        size_t start_pos = 0;
        while((start_pos = find(before, start_pos)) != std::string::npos) {
            std::string::replace(start_pos, before.length(), after);
            start_pos += after.length();
        }
        return *this;
    }

    QString midRef(int pos, int len = -1) const {
        return mid(pos, len);
    }

    QString rightJustified(int width, QChar fill = ' ') const {
        if (static_cast<int>(size()) >= width) {
            return *this;
        }
        return QString(width - size(), fill) + *this;
    }

    QString toUpper() const {
        QString result(*this);
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    QStringList split(const QString& separator) const {
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

    bool isEmpty() const {
        return empty();
    }

};