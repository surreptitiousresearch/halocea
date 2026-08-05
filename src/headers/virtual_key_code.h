#pragma once
/* virtual_key_code — the on-screen virtual keyboard's key identifier: an index into the
 * virtual_keyboard tag's virtual_keys block for the 36 configurable character keys, plus 8
 * action keys. Stored one byte per grid cell in virtual_keyboard_layout_table.
 * DB anonymous enum _50D9824DA78FEC0B38C59598451CAE0E (verbatim). */
#ifndef VIRTUAL_KEY_CODE_H
#define VIRTUAL_KEY_CODE_H

enum virtual_key_code
{
    _vkey_1 = 0,
    _vkey_2 = 1,
    _vkey_3 = 2,
    _vkey_4 = 3,
    _vkey_5 = 4,
    _vkey_6 = 5,
    _vkey_7 = 6,
    _vkey_8 = 7,
    _vkey_9 = 8,
    _vkey_0 = 9,
    _vkey_a = 10,
    _vkey_b = 11,
    _vkey_c = 12,
    _vkey_d = 13,
    _vkey_e = 14,
    _vkey_f = 15,
    _vkey_g = 16,
    _vkey_h = 17,
    _vkey_i = 18,
    _vkey_j = 19,
    _vkey_k = 20,
    _vkey_l = 21,
    _vkey_m = 22,
    _vkey_n = 23,
    _vkey_o = 24,
    _vkey_p = 25,
    _vkey_q = 26,
    _vkey_r = 27,
    _vkey_s = 28,
    _vkey_t = 29,
    _vkey_u = 30,
    _vkey_v = 31,
    _vkey_w = 32,
    _vkey_x = 33,
    _vkey_y = 34,
    _vkey_z = 35,
    NUMBER_OF_CONFIGURABLE_VIRTUAL_KEYS = 36,
    _vkey_done = 36,
    _vkey_shift = 37,
    _vkey_caps = 38,
    _vkey_symbols = 39,
    _vkey_backspace = 40,
    _vkey_left = 41,
    _vkey_right = 42,
    _vkey_space = 43,
    NUMBER_OF_VIRTUAL_KEYS = 44
};

#endif /* VIRTUAL_KEY_CODE_H */
