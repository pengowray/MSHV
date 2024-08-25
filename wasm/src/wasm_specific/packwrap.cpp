#include "../mshv_adapted/pack_unpack_msg.h"
#include "../mshv_adapted/pack_unpack_msg77.h"
#include "../wasm_specific/QString.h"
#include "../wasm_specific/QStringList.h"
#include <emscripten.h>

//PackUnpackMsg packunpack;
//PackUnpackMsg77 packunpack77;

extern "C" {

EMSCRIPTEN_KEEPALIVE
void init_ft8() {

}

EMSCRIPTEN_KEEPALIVE
const char* pack_ft8_message(const char* message) {
    static char packed_message[78]; // 77 + null terminator
    bool c77[77];
    //int i3 = -1, n3 = -1; // ignored anyway unless i3==0 && n3==5
    int i3 = 0, n3 = 0; // ignored anyway unless i3==0 && n3==5
    
    PackUnpackMsg77 packunpack77;
    const bool decoding = false; // false: generating/encoding/packing
    packunpack77.initPackUnpack77(decoding);
    packunpack77.pack77(QString(message), i3, n3, c77);

    if (i3 == -1) {
        strcpy(packed_message, "Pack failed");
        return packed_message;
    }

    for (int i = 0; i < 77; i++) {
        packed_message[i] = c77[i] ? '1' : '0';
    }
    packed_message[77] = '\0';
    
    return packed_message;
}

EMSCRIPTEN_KEEPALIVE
const char* unpack_ft8_message(const char* packed_message) {
    const int MAXLEN = 80; // was 50
    static char unpacked_message[MAXLEN];
    bool c77[77];
    bool unpk77_success = false;
    
    for (int i = 0; i < 77; i++) {
        c77[i] = (packed_message[i] == '1');
    }

    PackUnpackMsg77 packunpack77;
    const bool decoding = true; // decoding/unpacking
    packunpack77.initPackUnpack77(decoding);
    QString result = packunpack77.unpack77(c77, unpk77_success);
    
    if (unpk77_success) {
        std::string str = result;
        strncpy(unpacked_message, str.c_str(), MAXLEN-1);
        unpacked_message[MAXLEN-1] = '\0';
    } else {
        strcpy(unpacked_message, "Unpack failed");
    }
    
    return unpacked_message;
}

EMSCRIPTEN_KEEPALIVE
void save_hash_call(const char* call) {
    int n10, n12, n22;
    //TODO
    //packunpack77.save_hash_call(QString(call), n10, n12, n22);
    return;
}

}
