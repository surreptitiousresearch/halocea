#include "psBOOL.h"
#include <string.h>
// 0x82A312B0 — recognize one of the six boolean spellings (case-insensitive): "no"/"yes" (style 1),
// "off"/"on" (style 2), "false"/"true" (style 0). On a match, `*data`'s low byte gets the boolean
// value and the next byte gets the matched style; returns 1. On no match, `*data` is left untouched
// and the function returns 0.

// strnicmp comes from <string.h> (MSVC CRT); local redeclaration removed (C2733 extern "C" clash).
#define strnicmp _strnicmp

int psBOOL::IsThisBoolString(const char *str, int len, unsigned int *data)
{
    int matched = 0;
    unsigned char value = 0;
    unsigned char style = 0;

    if ((unsigned int)(len - 2) <= 3) { // len in [2,5]
        if (len == 2) {
            if (strnicmp(str, "no", 2) == 0) {
                value = 0; style = 1; matched = 1;
            } else if (strnicmp(str, "on", 2) == 0) {
                value = 1; style = 2; matched = 1;
            }
        } else if (len == 3) {
            if (strnicmp(str, "yes", 3) == 0) {
                value = 1; style = 1; matched = 1;
            } else if (strnicmp(str, "off", 3) == 0) {
                value = 0; style = 2; matched = 1;
            }
        } else if (len == 4) {
            if (strnicmp(str, "true", 4) == 0) {
                value = 1; style = 0; matched = 1;
            }
        } else if (len == 5) {
            if (strnicmp(str, "false", 5) == 0) {
                value = 0; style = 0; matched = 1;
            }
        }
    }

    if (data) {
        unsigned char *bytes = (unsigned char *)data;
        bytes[0] = value;
        bytes[1] = style;
    }
    return matched;
}
