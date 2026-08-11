/* virtual_keyboard_is_input_valid @0x837834D0 — decide whether a typed character is acceptable for a
 * given on-screen-keyboard field mode. Mode 4 (numeric/address) accepts '.', '-', ':' and any
 * alphanumeric/hex character; mode 5 accepts only digits; all other modes accept any character that is
 * not in the global invalid_keys blacklist. */

#include <stdint.h>
#include <string.h>
#include "headers/virtual_keyboard_validate_mode.h"
#include "headers/blam_data_globals.h"

extern unsigned short *__pctype_func(void);   /* CRT ctype table accessor */

uint8_t virtual_keyboard_is_input_valid(int ascii_code, int validate_mode)
{
    if (validate_mode == _virtual_keyboard_validate_internet_address)
    {
        switch ((uint8_t)ascii_code)
        {
            case '.':
            case '-':
            case ':':
                return 1;
        }
        const unsigned short *ctype = __pctype_func();
        return (ctype[(uint8_t)ascii_code] & 0x107) != 0;
    }
    else if (validate_mode == _virtual_keyboard_validate_number)
    {
        return __pctype_func()[(uint8_t)ascii_code] & 4;   /* 0x4 = digit */
    }
    else
    {
        return strchr(invalid_keys, (uint8_t)ascii_code) == nullptr;
    }
}
