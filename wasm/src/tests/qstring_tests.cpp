#ifdef QT_IMPL
#include <QString>
#include <QStringList>
#else
#include "../qtext_simple/QString.h"
#include "../qtext_simple/QStringList.h"
#endif

#include <cassert>
#include <iostream>
#include <climits>

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
    //std::cout << "str3: " << result3.count() << std::endl;
    //std::cout << "str3: " << "'" << result3[0] << "," << result3[1] << "," << result3[2] << "'\n\n";
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

    // Additional split test
    QString str6 = "a,,,b,c";
    QStringList result6 = str6.split(',', Qt::KeepEmptyParts);
    assert(result6.count() == 5);
    assert(result6[0] == "a");
    assert(result6[1] == "");
    assert(result6[2] == "");
    assert(result6[3] == "b");
    assert(result6[4] == "c");

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

void test_toInt() {
    QString str1 = "123";
    bool ok;
    int result = str1.toInt(&ok);
    assert(ok);
    assert(result == 123);

    QString str2 = "-456";
    result = str2.toInt(&ok);
    assert(ok);
    assert(result == -456);

    QString str3 = "abc";
    result = str3.toInt(&ok);
    assert(!ok);
    assert(result == 0);

    QString str4 = "";
    result = str4.toInt(&ok);
    assert(!ok);
    assert(result == 0);

    QString str5 = "123abc";
    result = str5.toInt(&ok);
    assert(!ok);
    assert(result == 0);

    QString str6 = "  42  ";
    result = str6.toInt(&ok);
    assert(ok);
    assert(result == 42);

    std::cout << "toInt tests passed" << std::endl;
}

void test_left() {
    QString str = "Hello, World!";
    assert(str.left(5) == "Hello");
    assert(str.left(0) == "");
    assert(str.left(20) == "Hello, World!");
    //std::cout << "\n\n" << str.left(-1).toStdString() << "\n\n";
    assert(str.left(-1) == "Hello, World!");
    assert(str.left(-5) == "Hello, World!");

    std::cout << "left tests passed" << std::endl;
}

void test_operator_index() {
    QString str = "abc";
    assert(str[0] == 'a');
    assert(str[1] == 'b');
    assert(str[2] == 'c');
    assert(str[3] == '\0');  // End of range (gives warning)
    assert(str[4] == '\0');  // Out of range access should return null character (gives warning)
    //assert(str[-1] == '\0');  // raises ASSERT: "i >= 0"

    std::cout << "operator[] tests passed" << std::endl;
}

int main() {
    test_split();
    test_trimmed();
    test_mid();
    test_toInt();
    test_left();
    test_operator_index();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
