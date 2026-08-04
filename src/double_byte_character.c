#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/language_code.h"
/* double_byte_character @ 0x83814DA0 — does the byte at `string` begin a two-byte unit? True for
 * the engine's "|x" inline format codes, and for the lead-byte ranges of the active CJK
 * multi-byte encoding (selected by global_language_code; the byte ranges are the raw DBCS
 * lead/trail bounds of each codepage — GB2312 / Big5 / Wansung / Johab / Shift-JIS). */

extern char *strchr(const char *s, int c);

uint8_t double_byte_character(const unsigned char *string)
{
    unsigned int lead = string[0];
    unsigned char trail;

    if ( !string[0] )
        return 0;
    trail = string[1];

    /* inline format escape: '|' followed by one of these codes */
    if ( lead == '|' && string[1] && strchr("ibukprlctn", string[1]) )
        return 1;

    if ( (unsigned int)(global_language_code - 1) > 4 )
        return 0;

    switch ( global_language_code )
    {
        case _language_simple_chinese:   /* GB2312/EUC-CN */
            if ( lead < 0xA1 || lead > 0xFE )
                return 0;
            return (trail >= 0xA1 && trail <= 0xFE) ? 1 : 0;

        case _language_traditional_chinese:   /* Big5 */
            if ( lead < 0x81 || lead > 0xFE )
                return 0;
            if ( trail >= 0x40 && trail <= 0x7E )
                return 1;
            return (trail >= 0xA1 && trail <= 0xFE) ? 1 : 0;

        case _language_korean_wansung:   /* UHC/Wansung */
            if ( lead < 0x81 || lead > 0xFE )
                return 0;
            if ( trail >= 0x41 && trail <= 0x5A )
                return 1;
            if ( trail >= 0x61 && trail <= 0x7A )
                return 1;
            return (trail >= 0x81 && trail <= 0xFE) ? 1 : 0;
    }

    if ( global_language_code != _language_japanese )
    {
        /* _language_korean_johab */
        if ( (lead < 0x84 || lead > 0xD3) && (lead < 0xD8 || lead > 0xDE) && (lead < 0xE0 || lead > 0xF9) )
            return 0;
        if ( trail >= 0x41 && trail <= 0x7E )
            return 1;
        return (trail >= 0x81 && trail <= 0xFE) ? 1 : 0;
    }

    /* _language_japanese — Shift-JIS */
    if ( ((lead >= 0x81 && lead <= 0x9F) || (lead >= 0xE0 && lead <= 0xFE))
      && trail >= 0x40 && trail <= 0xFC && trail != 0x7F )
        return 1;
    return 0;
}
