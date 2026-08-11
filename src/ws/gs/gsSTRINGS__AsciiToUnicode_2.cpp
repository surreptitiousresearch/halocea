#include "../../headers/ws/gs/gsSTRINGS.h"

// Module-local scratch buffer backing the single-argument AsciiToUnicode convenience overload.
static wchar_t unicodeBuffer[0x2000];

// @0x826B4EA8 — widen `text` into unicodeBuffer and return it.
const wchar_t *gsSTRINGS::AsciiToUnicode(const char *text)
{
    AsciiToUnicode(text, unicodeBuffer, 0x2000);
    return unicodeBuffer;
}
