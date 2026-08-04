#pragma once
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* _XINPUT_KEYSTROKE — Xbox XInput keystroke event (8 bytes, DB layout / standard XINPUT_KEYSTROKE). */

typedef struct _XINPUT_KEYSTROKE
{
    unsigned __int16 VirtualKey; /* 0x0 */
    wchar_t          Unicode;    /* 0x2 */
    unsigned __int16 Flags;      /* 0x4 */
    unsigned __int8  UserIndex;  /* 0x6 */
    unsigned __int8  HidCode;    /* 0x7 */
} _XINPUT_KEYSTROKE;             /* 8 bytes */
