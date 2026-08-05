#pragma once
#include <stdint.h>
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* _XINPUT_KEYSTROKE — Xbox XInput keystroke event (8 bytes, DB layout / standard XINPUT_KEYSTROKE). */

typedef struct _XINPUT_KEYSTROKE
{
    uint16_t VirtualKey; /* 0x0 */
    wchar_t          Unicode;    /* 0x2 */
    uint16_t Flags;      /* 0x4 */
    uint8_t  UserIndex;  /* 0x6 */
    uint8_t  HidCode;    /* 0x7 */
} _XINPUT_KEYSTROKE;             /* 8 bytes */
