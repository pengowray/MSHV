#include "../mshv_adapted/pack_unpack_msg77.h"
#include <cassert>
#include <iostream>

void test_split77() {
    PackUnpackMsg77 packunpack77;
    int nwords;
    QString w[19];

    // Test case 1
    QString msg1 = "CQ K1ABC FN42";
    packunpack77.split77(msg1, nwords, w);
    std::cout << "\n\nmsg1: '" << w[0].toStdString() << "," << w[1].toStdString() << "," << w[2].toStdString() << "'\n\n";
    assert(nwords == 3);
    assert(w[0] == "CQ");
    assert(w[1] == "K1ABC");
    assert(w[2] == "FN42");

    // Test case 2
    QString msg2 = "K1ABC W9XYZ -15";
    packunpack77.split77(msg2, nwords, w);
    std::cout << "\n\nmsg2: '" << w[0].toStdString() << "," << w[1].toStdString() << "," << w[2].toStdString() << "'\n\n";
    assert(nwords == 3);
    assert(w[0] == "K1ABC");
    assert(w[1] == "W9XYZ");
    assert(w[2] == "-15");

    // Test case 3
    QString msg3 = "CQ DX K1ABC FN42";
    packunpack77.split77(msg3, nwords, w);
    assert(nwords == 4);
    assert(w[0] == "CQ");
    assert(w[1] == "DX");
    assert(w[2] == "K1ABC");
    assert(w[3] == "FN42");

    std::cout << "split77 tests passed" << std::endl;
}

void test_pack77() {
    PackUnpackMsg77 packunpack77;
    packunpack77.initPackUnpack77(false);  // Set to false for generating/encoding/packing
    bool c77[77];
    int i3, n3;

    // Test case 1
    QString msg1 = "CQ K1ABC FN42";
    packunpack77.pack77(msg1, i3, n3, c77);
    // Add assertions for expected packed message (you'll need to determine the correct output)
    assert(i3 == 1);  // Assuming this should be a Type 1 message
    // Add more assertions for c77 contents

    // Test case 2
    QString msg2 = "K1ABC W9XYZ -15";
    packunpack77.pack77(msg2, i3, n3, c77);
    assert(i3 == 1);  // Assuming this should be a Type 1 message
    // Add more assertions for c77 contents

    // Test case 3
    QString msg3 = "CQ DX K1ABC FN42";
    packunpack77.pack77(msg3, i3, n3, c77);
    assert(i3 == 1);  // Assuming this should be a Type 1 message
    // Add more assertions for c77 contents

    // Test case 4 (free text message)
    QString msg4 = "THIS IS A FREE TEXT MESSAGE";
    packunpack77.pack77(msg4, i3, n3, c77);
    assert(i3 == 0);  // This should be a Type 0 (free text) message
    assert(n3 == 0);
    // Add more assertions for c77 contents

    std::cout << "pack77 tests passed" << std::endl;
}

int main() {
    test_split77();
    test_pack77();
    std::cout << "All PackUnpackMsg77 tests passed!" << std::endl;
    return 0;
}