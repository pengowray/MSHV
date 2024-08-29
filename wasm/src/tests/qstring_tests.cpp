#ifdef QT_IMPL
#include <QString>
#include <QStringList>
#else
#include "../qtext_simple/QString.h"
#include "../qtext_simple/QStringList.h"
#endif

#include <cassert>
#include <iostream>

void test_split() {
    QString str1 = "hello world";
    auto result1 = str1.split(" ");
    assert(result1.count() == 2);
    assert(result1[0] == "hello");
    assert(result1[1] == "world");

    QString str2 = "a  b c   d";
    auto result2 = str2.split(" ", Qt::SkipEmptyParts);
    assert(result2.count() == 4);
    assert(result2[0] == "a");
    assert(result2[1] == "b");
    assert(result2[2] == "c");
    assert(result2[3] == "d");

    QString str2b = "a  b c   d";
    auto result2b = str2.split(" ");
    //std::cout << "str2b: " << result2b.count() << std::endl;
    assert(result2.count() == 4);
    assert(result2[0] == "a");
    assert(result2[1] == "b");
    assert(result2[2] == "c");
    assert(result2[3] == "d");

    QString str3 = " trim me ";
    auto result3 = str3.split(" ");
    assert(result3.count() == 4);
    assert(result3[0] == "");
    assert(result3[1] == "trim");
    assert(result3[2] == "me");
    assert(result3[3] == "");

    QString str3b = " trim ";
    auto result3b = str3b.split(" ", Qt::SkipEmptyParts);
    //std::cout << Qt::SkipEmptyParts << ", " << Qt::KeepEmptyParts; // 1, 0
    assert(result3b.count() == 1);
    assert(result3b[0] == "trim");
    
    // parts: {"", "a", "b", "c", ""}
    QString str4 = "abc";
    auto result4 = str4.split(QString());
    assert(result4.count() == 5);  // Should only contain "trim" and "me"
    assert(result4[0] == "");
    assert(result4[1] == "a");
    assert(result4[2] == "b");
    assert(result4[3] == "c");
    assert(result4[4] == "");

    QString str5 = "a,,b,c";
    QStringList result5a = str5.split(',', Qt::KeepEmptyParts);   // Returns ["a", "", "b", "c"]
    QStringList result5b = str5.split(',', Qt::SkipEmptyParts);   // Returns ["a", "b", "c"]
    assert(result5a.count() == 4);
    assert(result5b.count() == 3);

    std::cout << "Split tests passed" << std::endl;
}

void test_trimmed() {
    QString str1 = "  hello  ";
    assert(str1.trimmed() == "hello");

    QString str2 = "no trim needed";
    assert(str2.trimmed() == "no trim needed");

    QString str3 = " \t spaces and tabs \t ";
    assert(str3.trimmed() == "spaces and tabs");

    std::cout << "Trimmed tests passed" << std::endl;
}

void test_mid() {
    QString str = "hello world";
    assert(str.mid(0, 5) == "hello");
    assert(str.mid(6) == "world");
    assert(str.mid(3, 3) == "lo ");
    assert(str.mid(20) == "");

    std::cout << "Mid tests passed" << std::endl;
}

int main() {
    test_split();
    test_trimmed();
    test_mid();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
