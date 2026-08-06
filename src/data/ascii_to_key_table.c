/* ascii_to_key_table @ 0x82114410 (.rdata, 256 bytes)
 * DB applied_types: const __int16 ascii_to_key_table[128];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0010: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0020: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0030: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0040: 0048 0011 0037 0013 0014 0015 0017 0037
 *   +0x0050: 0019 001A 0018 001C 0041 001B 0042 0043
 *   +0x0060: 001A 0011 0012 0013 0014 0015 0016 0017
 *   +0x0070: 0018 0019 0036 0036 0041 001C 0042 0043
 *   +0x0080: 0012 002D 003E 003C 002F 0021 0030 0031
 *   +0x0090: 0032 0026 0033 0034 0035 0040 003F 0027
 *   +0x00A0: 0028 001F 0022 002E 0023 0025 003D 0020
 *   +0x00B0: 003B 0024 003A 0029 002B 002A 0016 001B
 *   +0x00C0: 0010 002D 003E 003C 002F 0021 0030 0031
 *   +0x00D0: 0032 0026 0033 0034 0035 0040 003F 0027
 *   +0x00E0: 0028 001F 0022 002E 0023 0025 003D 0020
 *   +0x00F0: 003B 0024 003A 0029 002B 002A 0010 0054
 * .rdata, 256 bytes = 128 halfwords; DB applied type
 * `const __int16[128]`, matching the declaration in src/input_update_keyboard.c
 * (`extern const int16_t ascii_to_key_table[128]`). 256 bytes over the 128 ASCII codes is 2 bytes
 * per entry, and the element is a key_code as in the sibling virtual_to_key_table.
 * input_update_keyboard uses it only as a validity test - a keystroke whose Unicode value is below
 * 0x80 keeps its ascii_code when this table has a mapping for it, and is replaced by -1 when the
 * entry is the 0xFFFF sentinel. 96 of the 128 slots are mapped: the printable range 0x20-0x7E plus
 * DEL, with each entry naming the physical key that produces the character (shifted characters map
 * to their unshifted key, so '!' -> _key_1 and ':' -> _key_semicolon).
 * Reconstructed from the raw big-endian halfwords; the mapping is self-checking - the digits, the
 * letters (upper and lower case both landing on the same key_code) and the shifted punctuation all
 * follow the US keyboard layout exactly.
 */
#include <stdint.h>
#include "../headers/key_code.h"

const int16_t ascii_to_key_table[128] =
{
    /*   0-11   */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    /*  12-23   */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    /*  24-31   */ -1, -1, -1, -1, -1, -1, -1, -1,
    /*  32 SP   */ _key_space,
    /*  33 '!'  */ _key_1,
    /*  34 '"'  */ _key_apostrophe,
    /*  35 '#'  */ _key_3,
    /*  36 '$'  */ _key_4,
    /*  37 '%'  */ _key_5,
    /*  38 '&'  */ _key_7,
    /*  39 '''  */ _key_apostrophe,
    /*  40 '('  */ _key_9,
    /*  41 ')'  */ _key_0,
    /*  42 '*'  */ _key_8,
    /*  43 '+'  */ _key_equal,
    /*  44 ','  */ _key_comma,
    /*  45 '-'  */ _key_dash,
    /*  46 '.'  */ _key_period,
    /*  47 '/'  */ _key_forwardslash,
    /*  48 '0'  */ _key_0,
    /*  49 '1'  */ _key_1,
    /*  50 '2'  */ _key_2,
    /*  51 '3'  */ _key_3,
    /*  52 '4'  */ _key_4,
    /*  53 '5'  */ _key_5,
    /*  54 '6'  */ _key_6,
    /*  55 '7'  */ _key_7,
    /*  56 '8'  */ _key_8,
    /*  57 '9'  */ _key_9,
    /*  58 ':'  */ _key_semicolon,
    /*  59 ';'  */ _key_semicolon,
    /*  60 '<'  */ _key_comma,
    /*  61 '='  */ _key_equal,
    /*  62 '>'  */ _key_period,
    /*  63 '?'  */ _key_forwardslash,
    /*  64 '@'  */ _key_2,
    /*  65 'A'  */ _key_a,
    /*  66 'B'  */ _key_b,
    /*  67 'C'  */ _key_c,
    /*  68 'D'  */ _key_d,
    /*  69 'E'  */ _key_e,
    /*  70 'F'  */ _key_f,
    /*  71 'G'  */ _key_g,
    /*  72 'H'  */ _key_h,
    /*  73 'I'  */ _key_i,
    /*  74 'J'  */ _key_j,
    /*  75 'K'  */ _key_k,
    /*  76 'L'  */ _key_l,
    /*  77 'M'  */ _key_m,
    /*  78 'N'  */ _key_n,
    /*  79 'O'  */ _key_o,
    /*  80 'P'  */ _key_p,
    /*  81 'Q'  */ _key_q,
    /*  82 'R'  */ _key_r,
    /*  83 'S'  */ _key_s,
    /*  84 'T'  */ _key_t,
    /*  85 'U'  */ _key_u,
    /*  86 'V'  */ _key_v,
    /*  87 'W'  */ _key_w,
    /*  88 'X'  */ _key_x,
    /*  89 'Y'  */ _key_y,
    /*  90 'Z'  */ _key_z,
    /*  91 '['  */ _key_left_bracket,
    /*  92 '\'  */ _key_backslash,
    /*  93 ']'  */ _key_right_bracket,
    /*  94 '^'  */ _key_6,
    /*  95 '_'  */ _key_dash,
    /*  96 '`'  */ _key_backquote,
    /*  97 'a'  */ _key_a,
    /*  98 'b'  */ _key_b,
    /*  99 'c'  */ _key_c,
    /* 100 'd'  */ _key_d,
    /* 101 'e'  */ _key_e,
    /* 102 'f'  */ _key_f,
    /* 103 'g'  */ _key_g,
    /* 104 'h'  */ _key_h,
    /* 105 'i'  */ _key_i,
    /* 106 'j'  */ _key_j,
    /* 107 'k'  */ _key_k,
    /* 108 'l'  */ _key_l,
    /* 109 'm'  */ _key_m,
    /* 110 'n'  */ _key_n,
    /* 111 'o'  */ _key_o,
    /* 112 'p'  */ _key_p,
    /* 113 'q'  */ _key_q,
    /* 114 'r'  */ _key_r,
    /* 115 's'  */ _key_s,
    /* 116 't'  */ _key_t,
    /* 117 'u'  */ _key_u,
    /* 118 'v'  */ _key_v,
    /* 119 'w'  */ _key_w,
    /* 120 'x'  */ _key_x,
    /* 121 'y'  */ _key_y,
    /* 122 'z'  */ _key_z,
    /* 123 '{'  */ _key_left_bracket,
    /* 124 '|'  */ _key_backslash,
    /* 125 '}'  */ _key_right_bracket,
    /* 126 '~'  */ _key_backquote,
    /* 127      */ _key_delete
};
