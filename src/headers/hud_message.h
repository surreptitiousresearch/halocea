#pragma once
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* hud_message — one HUD message slot (140 bytes): pickup/objective text plus its timing and item info. */

typedef struct hud_message
{
    int             time;                  /* 0x00 */
    wchar_t         msg[63];               /* 0x04 */
    unsigned char   valid;                 /* 0x82 */
    unsigned char   magic_number;          /* 0x83 */
    int             item_definition_index; /* 0x84 */
    short           quantity;              /* 0x88 */
    char            message_offset;        /* 0x8A */
    unsigned char   _pad8B;                /* 0x8B */
} hud_message;                             /* 0x8C */
