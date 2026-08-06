/* virtual_to_key_table @ 0x82114210 (.rdata, 512 bytes)
 * DB applied_types: const __int16 virtual_to_key_table[256];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0010: 001D 001E FFFF FFFF FFFF 0038 FFFF FFFF
 *   +0x0020: 006E 006F 0047 000F 002C FFFF FFFF FFFF
 *   +0x0030: FFFF FFFF FFFF 0000 FFFF FFFF FFFF FFFF
 *   +0x0040: 0048 0053 0056 0055 0052 004F 004D 0050
 *   +0x0050: 004E FFFF FFFF FFFF FFFF 0051 0054 FFFF
 *   +0x0060: 001A 0011 0012 0013 0014 0015 0016 0017
 *   +0x0070: 0018 0019 FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0080: FFFF 002D 003E 003C 002F 0021 0030 0031
 *   +0x0090: 0032 0026 0033 0034 0035 0040 003F 0027
 *   +0x00A0: 0028 001F 0022 002E 0023 0025 003D 0020
 *   +0x00B0: 003B 0024 003A 0046 004A 004B FFFF FFFF
 *   +0x00C0: 005A 005B 005C 005D 005E 005F 0060 0061
 *   +0x00D0: 0062 0063 0059 0065 FFFF 0064 0067 0058
 *   +0x00E0: 0001 0002 0003 0004 0005 0006 0007 0008
 *   +0x00F0: 0009 000A 000B 000C FFFF FFFF FFFF FFFF
 *   +0x0100: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0110: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0120: 0057 000E FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0130: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0140: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0150: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0160: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0170: FFFF FFFF 0036 001C 0041 001B 0042 0043
 *   +0x0180: 0010 FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0190: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x01A0: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x01B0: FFFF FFFF FFFF 0029 002B 002A 0037 FFFF
 *   +0x01C0: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x01D0: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x01E0: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x01F0: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 * .rdata, 512 bytes = 256 halfwords; DB applied type
 * `const __int16[256]`, matching the declaration in src/input_update_keyboard.c
 * (`extern const int16_t virtual_to_key_table[256]`). 512 bytes over the 256 Windows virtual-key
 * codes is 2 bytes per entry, and the consumer's `input_globals.key_latches[stroke.key_code]`
 * indexing makes the element a key_code.
 * input_update_keyboard reads it as virtual_to_key_table[keystroke.VirtualKey] to turn an
 * XINPUT_KEYSTROKE virtual-key code into a Blam key_code. -1 marks an unmapped virtual key;
 * the consumer's `(uint16_t)ascii_to_key_table[...] == 0xFFFF` test is the same sentinel read
 * unsigned. 99 of the 256 slots are mapped.
 * Reconstructed from the raw big-endian halfwords; every mapped slot agrees with the standard
 * Windows VK_ assignment named in its comment (VK_BACK -> _key_backspace, VK_OEM_3 -> _key_backquote
 * and so on), which is the independent cross-check on the decode.
 * CAVEAT (original data, reproduced as-is): VK_SHIFT and VK_CONTROL map to _key_shift (110) and
 * _key_control (111), the two synthetic aggregate codes above NUMBER_OF_KEYS, so input_update_keyboard's
 * `key_code <= _key_102_oem` guard drops those two keystrokes entirely; VK_MENU by contrast maps to
 * the concrete _key_left_alt (71) and is accepted.
 */
#include <stdint.h>
#include "../headers/key_code.h"

const int16_t virtual_to_key_table[256] =
{
    /* 0x00-0x07        */ -1, -1, -1, -1, -1, -1, -1, -1,
    /* 0x08 VK_BACK     */ _key_backspace,
    /* 0x09 VK_TAB      */ _key_tab,
    /* 0x0A-0x0C        */ -1, -1, -1,
    /* 0x0D VK_RETURN   */ _key_return,
    /* 0x0E-0x0F        */ -1, -1,
    /* 0x10 VK_SHIFT    */ _key_shift,
    /* 0x11 VK_CONTROL  */ _key_control,
    /* 0x12 VK_MENU     */ _key_left_alt,
    /* 0x13 VK_PAUSE    */ _key_pause,
    /* 0x14 VK_CAPITAL  */ _key_caps_lock,
    /* 0x15-0x1A        */ -1, -1, -1, -1, -1, -1,
    /* 0x1B VK_ESCAPE   */ _key_escape,
    /* 0x1C-0x1F        */ -1, -1, -1, -1,
    /* 0x20 VK_SPACE    */ _key_space,
    /* 0x21 VK_PRIOR    */ _key_page_up,
    /* 0x22 VK_NEXT     */ _key_page_down,
    /* 0x23 VK_END      */ _key_end,
    /* 0x24 VK_HOME     */ _key_home,
    /* 0x25 VK_LEFT     */ _key_left_arrow,
    /* 0x26 VK_UP       */ _key_up_arrow,
    /* 0x27 VK_RIGHT    */ _key_right_arrow,
    /* 0x28 VK_DOWN     */ _key_down_arrow,
    /* 0x29-0x2C        */ -1, -1, -1, -1,
    /* 0x2D VK_INSERT   */ _key_insert,
    /* 0x2E VK_DELETE   */ _key_delete,
    /* 0x2F             */ -1,
    /* 0x30 VK_'0'      */ _key_0,
    /* 0x31 VK_'1'      */ _key_1,
    /* 0x32 VK_'2'      */ _key_2,
    /* 0x33 VK_'3'      */ _key_3,
    /* 0x34 VK_'4'      */ _key_4,
    /* 0x35 VK_'5'      */ _key_5,
    /* 0x36 VK_'6'      */ _key_6,
    /* 0x37 VK_'7'      */ _key_7,
    /* 0x38 VK_'8'      */ _key_8,
    /* 0x39 VK_'9'      */ _key_9,
    /* 0x3A-0x40        */ -1, -1, -1, -1, -1, -1, -1,
    /* 0x41 VK_'A'      */ _key_a,
    /* 0x42 VK_'B'      */ _key_b,
    /* 0x43 VK_'C'      */ _key_c,
    /* 0x44 VK_'D'      */ _key_d,
    /* 0x45 VK_'E'      */ _key_e,
    /* 0x46 VK_'F'      */ _key_f,
    /* 0x47 VK_'G'      */ _key_g,
    /* 0x48 VK_'H'      */ _key_h,
    /* 0x49 VK_'I'      */ _key_i,
    /* 0x4A VK_'J'      */ _key_j,
    /* 0x4B VK_'K'      */ _key_k,
    /* 0x4C VK_'L'      */ _key_l,
    /* 0x4D VK_'M'      */ _key_m,
    /* 0x4E VK_'N'      */ _key_n,
    /* 0x4F VK_'O'      */ _key_o,
    /* 0x50 VK_'P'      */ _key_p,
    /* 0x51 VK_'Q'      */ _key_q,
    /* 0x52 VK_'R'      */ _key_r,
    /* 0x53 VK_'S'      */ _key_s,
    /* 0x54 VK_'T'      */ _key_t,
    /* 0x55 VK_'U'      */ _key_u,
    /* 0x56 VK_'V'      */ _key_v,
    /* 0x57 VK_'W'      */ _key_w,
    /* 0x58 VK_'X'      */ _key_x,
    /* 0x59 VK_'Y'      */ _key_y,
    /* 0x5A VK_'Z'      */ _key_z,
    /* 0x5B VK_LWIN     */ _key_left_windows,
    /* 0x5C VK_RWIN     */ _key_right_windows,
    /* 0x5D VK_APPS     */ _key_menu,
    /* 0x5E-0x5F        */ -1, -1,
    /* 0x60 VK_NUMPAD0  */ _keypad_0,
    /* 0x61 VK_NUMPAD1  */ _keypad_1,
    /* 0x62 VK_NUMPAD2  */ _keypad_2,
    /* 0x63 VK_NUMPAD3  */ _keypad_3,
    /* 0x64 VK_NUMPAD4  */ _keypad_4,
    /* 0x65 VK_NUMPAD5  */ _keypad_5,
    /* 0x66 VK_NUMPAD6  */ _keypad_6,
    /* 0x67 VK_NUMPAD7  */ _keypad_7,
    /* 0x68 VK_NUMPAD8  */ _keypad_8,
    /* 0x69 VK_NUMPAD9  */ _keypad_9,
    /* 0x6A VK_MULTIPLY */ _keypad_multiply,
    /* 0x6B VK_ADD      */ _keypad_add,
    /* 0x6C             */ -1,
    /* 0x6D VK_SUBTRACT */ _keypad_subtract,
    /* 0x6E VK_DECIMAL  */ _keypad_decimal,
    /* 0x6F VK_DIVIDE   */ _keypad_divide,
    /* 0x70 VK_F1       */ _key_f1,
    /* 0x71 VK_F2       */ _key_f2,
    /* 0x72 VK_F3       */ _key_f3,
    /* 0x73 VK_F4       */ _key_f4,
    /* 0x74 VK_F5       */ _key_f5,
    /* 0x75 VK_F6       */ _key_f6,
    /* 0x76 VK_F7       */ _key_f7,
    /* 0x77 VK_F8       */ _key_f8,
    /* 0x78 VK_F9       */ _key_f9,
    /* 0x79 VK_F10      */ _key_f10,
    /* 0x7A VK_F11      */ _key_f11,
    /* 0x7B VK_F12      */ _key_f12,
    /* 0x7C-0x87        */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    /* 0x88-0x8F        */ -1, -1, -1, -1, -1, -1, -1, -1,
    /* 0x90 VK_NUMLOCK  */ _keypad_num_lock,
    /* 0x91 VK_SCROLL   */ _key_scroll_lock,
    /* 0x92-0x9D        */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    /* 0x9E-0xA9        */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    /* 0xAA-0xB5        */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    /* 0xB6-0xB9        */ -1, -1, -1, -1,
    /* 0xBA VK_OEM_1    */ _key_semicolon,
    /* 0xBB VK_OEM_PLUS */ _key_equal,
    /* 0xBC VK_OEM_COMMA */ _key_comma,
    /* 0xBD VK_OEM_MINUS */ _key_dash,
    /* 0xBE VK_OEM_PERIOD */ _key_period,
    /* 0xBF VK_OEM_2    */ _key_forwardslash,
    /* 0xC0 VK_OEM_3    */ _key_backquote,
    /* 0xC1-0xCC        */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    /* 0xCD-0xD8        */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    /* 0xD9-0xDA        */ -1, -1,
    /* 0xDB VK_OEM_4    */ _key_left_bracket,
    /* 0xDC VK_OEM_5    */ _key_backslash,
    /* 0xDD VK_OEM_6    */ _key_right_bracket,
    /* 0xDE VK_OEM_7    */ _key_apostrophe,
    /* 0xDF-0xEA        */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    /* 0xEB-0xF6        */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    /* 0xF7-0xFF        */ -1, -1, -1, -1, -1, -1, -1, -1, -1
};
