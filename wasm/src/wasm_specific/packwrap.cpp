#include "../qtext_simple/QString.h"
#include "../qtext_simple/QStringList.h"
#include "../qtext_simple/QChar.h"

#include "../mshv_adapted/pack_unpack_msg.h"
#include "../mshv_adapted/pack_unpack_msg77.h"

#include <emscripten.h>
#include <string.h>
#include <vector>

extern "C" {

#define MAXLEN 120 // return string (78 probably max needed)

struct PResult {
    int error_code;
    char message[MAXLEN];
};

// Vector of PackUnpackMsg77 instances
std::vector<PackUnpackMsg77*> packunpack77_instances;

EMSCRIPTEN_KEEPALIVE
int create_packunpack77_instance(bool forUnpacking = true) {
    PackUnpackMsg77* instance = new PackUnpackMsg77();
    instance->initPackUnpack77(forUnpacking);
    packunpack77_instances.push_back(instance);
    return packunpack77_instances.size() - 1;
}

EMSCRIPTEN_KEEPALIVE
void destroy_packunpack77_instance(int index) {
    if (index > 0 && index < packunpack77_instances.size()) {
        delete packunpack77_instances[index];
        packunpack77_instances[index] = nullptr;
    }
}

EMSCRIPTEN_KEEPALIVE
void init_ft8() {
    // Create default instances
    create_packunpack77_instance(false); // 0: packing
    create_packunpack77_instance(true); // 1: unpacking
}

EMSCRIPTEN_KEEPALIVE
PResult* pack_ft8_message(const char* message, int instance_index = 0) {
    static PResult result;
    bool c77[77];
    int i3 = -1, n3 = -1;

    if (!message) {
        strcpy(result.message, "Null message pointer");
        result.error_code = 2;
        return &result;
    }

    if (instance_index < 0 || instance_index >= packunpack77_instances.size() || !packunpack77_instances[instance_index]) {
        strcpy(result.message, "Invalid PackUnpackMsg77 instance");
        result.error_code = 3;
        return &result;
    }

    PackUnpackMsg77* packunpack77 = packunpack77_instances[instance_index];
    packunpack77->pack77(QString(message), i3, n3, c77);

    if (i3 == -1) {
        strcpy(result.message, "Pack failed");
        result.error_code = 1;
        return &result;
    }

    for (int i = 0; i < 77; i++) {
        result.message[i] = c77[i] ? '1' : '0';
    }
    result.message[77] = '\0';
    result.error_code = 0;

    return &result;
}

PResult *pack_ft8_message_safe(const char *message, int instance_index = 0)
{
    try {

        return pack_ft8_message(message, instance_index);

    } catch (const std::out_of_range &e) {
        EM_ASM_({ throw new Error('C++ out_of_range exception: ' + UTF8ToString($0)); }, e.what());
    } catch (const std::runtime_error &e) {
        EM_ASM_({ throw new Error('C++ runtime_error exception: ' + UTF8ToString($0)); }, e.what());
    } catch (const std::exception &e) {
        EM_ASM_({ throw new Error('C++ exception: ' + UTF8ToString($0)); }, e.what());
    } catch (...) {
        EM_ASM({ throw new Error('Unknown C++ exception occurred'); });
    }

    static PResult result;
    strcpy(result.message, "Unreachable error");
    result.error_code = -999;
    return &result;
}

EMSCRIPTEN_KEEPALIVE
PResult* unpack_ft8_message(const char* packed_message, int instance_index = 1) {
    static PResult result;
    bool c77[77];
    bool unpk77_success = false;

    if (instance_index < 0 || instance_index >= packunpack77_instances.size() || !packunpack77_instances[instance_index]) {
        strcpy(result.message, "Invalid PackUnpackMsg77 instance");
        result.error_code = 3;
        return &result;
    }

    for (int i = 0; i < 77; i++) {
        c77[i] = (packed_message[i] == '1');
    }

    PackUnpackMsg77* packunpack77 = packunpack77_instances[instance_index];
    QString qresult = packunpack77->unpack77(c77, unpk77_success);

    if (unpk77_success) {
        std::string str = qresult.toStdString();
        strncpy(result.message, str.c_str(), MAXLEN - 1);
        result.message[MAXLEN - 1] = '\0';
        result.error_code = 0;
    } else {
        strcpy(result.message, "Unpack failed");
        result.error_code = 1;
    }

    return &result;
}

EMSCRIPTEN_KEEPALIVE
void save_hash_call(const char* call, int instance_index = 1) {
    if (instance_index >= 0 && instance_index < packunpack77_instances.size() && packunpack77_instances[instance_index]) {
        PackUnpackMsg77* packunpack77 = packunpack77_instances[instance_index];
        int n10, n12, n22;
        packunpack77->save_hash_call(QString(call), n10, n12, n22);
    }
}

} // extern "C"