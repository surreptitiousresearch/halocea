#include "../headers/havok/hkError.h"

/* Emit a single error string to the global hkError sink; trap if the handler
   asks for a break. */
extern "C" void hkErrorMessage(const char *text)
{
    hkErrStream stream;
    char buffer[512];

    hkErrStream_construct(&stream, buffer, 512);
    hkOstream_insertString((hkOstream *)&stream, text);
    if (hkError_message(hkError_s_instance, MESSAGE_ERROR, 641138213, buffer,
                        ".\\System\\Error\\hkError.cpp", 38)) /* backslashes escaped 2026-07-30 */
    {
        __trap();
    }
    hkOstream_destruct((hkOstream *)&stream);
}
