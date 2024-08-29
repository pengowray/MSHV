#include "../qtext_simple/QString.h"
#include "../qtext_simple/QStringList.h"
#include "../qtext_simple/QChar.h"

#include "../mshv_adapted/pack_unpack_msg.h"
#include "../mshv_adapted/pack_unpack_msg77.h"

#include <emscripten.h>
#include <string.h>

extern "C"
{

#define MAXLEN 120 // return string (78 probably max needed)

    struct PResult
    {
        int error_code;
        char message[MAXLEN];
    };

    EMSCRIPTEN_KEEPALIVE
    void init_ft8()
    {
        // Initialization code if needed
    }

    EMSCRIPTEN_KEEPALIVE
    PResult *pack_ft8_message(const char *message) {
        static PResult result;
        bool c77[77];
        int i3 = 0, n3 = 0;

            if (!message)
            {
                printf("Error: Null message pointer\n");
                strcpy(result.message, "Null message pointer");
                result.error_code = 2;
                return &result;
            }

            PackUnpackMsg77 packunpack77;
            const bool decoding = false; // false: generating/encoding/packing
            packunpack77.initPackUnpack77(decoding);
            packunpack77.pack77(QString(message), i3, n3, c77);

            if (i3 == -1)
            {
                strcpy(result.message, "Pack failed");
                result.error_code = 1;
                return &result;
            }

            for (int i = 0; i < 77; i++)
            {
                result.message[i] = c77[i] ? '1' : '0';
            }
            result.message[77] = '\0';
            result.error_code = 0;

            return &result;
    }

    EMSCRIPTEN_KEEPALIVE
    PResult *pack_ft8_message_safe(const char *message)
    {
        try {

            return pack_ft8_message(message);

        } catch (const std::out_of_range &e) {
            EM_ASM_({ throw new Error('C++ out_of_range exception: ' + UTF8ToString($0)); }, e.what());
        } catch (const std::runtime_error &e) {
            EM_ASM_({ throw new Error('C++ runtime_error exception: ' + UTF8ToString($0)); }, e.what());
        } catch (const std::exception &e) {
            EM_ASM_({ throw new Error('C++ exception: ' + UTF8ToString($0)); }, e.what());
        } catch (...) {
            EM_ASM({ throw new Error('Unknown C++ exception occurred'); });
        }
    }

    EMSCRIPTEN_KEEPALIVE
    PResult *unpack_ft8_message(const char *packed_message)
    {
        static PResult result;
        bool c77[77];
        bool unpk77_success = false;

        for (int i = 0; i < 77; i++)
        {
            c77[i] = (packed_message[i] == '1');
        }

        PackUnpackMsg77 packunpack77;
        const bool decoding = true; // decoding/unpacking
        packunpack77.initPackUnpack77(decoding);
        QString qresult = packunpack77.unpack77(c77, unpk77_success);

        if (unpk77_success)
        {
            std::string str = qresult.toStdString();
            strncpy(result.message, str.c_str(), MAXLEN - 1);
            result.message[MAXLEN - 1] = '\0';
            result.error_code = 0;
        }
        else
        {
            strcpy(result.message, "Unpack failed");
            result.error_code = 1;
        }

        return &result;
    }

    EMSCRIPTEN_KEEPALIVE
    void save_hash_call(const char *call)
    {
        int n10, n12, n22;
        // TODO: Implement this function
        // packunpack77.save_hash_call(QString(call), n10, n12, n22);
    }
}