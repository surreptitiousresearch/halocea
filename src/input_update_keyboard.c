/* input_update_keyboard @0x836FB9B8 — advance the keyboard state for one frame. First ages every key's
 * held-tick counter (incremented while latched, saturating at 255; cleared when released), then drains the
 * XInput keystroke queue: each event's modifier flags (shift/ctrl/alt) and ascii/key codes are decoded,
 * key-up events clear the latch, and key-down events are appended to the buffered-key ring (up to 64) and
 * set the latch. Returns the final XInputGetKeystrokeEx status (nonzero once the queue is empty).
 *
 * Flag bits follow XINPUT_KEYSTROKE: KEYUP=0x2, SHIFT=0x8, CTRL=0x10, ALT=0x20. modifier_flags packs
 * shift(bit0) | ctrl(bit1) | alt(bit2). */

#include <stdint.h>
#include "headers/input_globals.h"
#include "headers/key_stroke.h"
#include "headers/key_modifier_flags.h"
#include "headers/xinput_keystroke.h"
#include "headers/blam_data_globals.h"
#include "headers/input_constants.h"
#include "headers/key_code.h"

extern const int16_t ascii_to_key_table[128];
extern const int16_t virtual_to_key_table[256];
extern unsigned int XInputGetKeystrokeEx(unsigned int *pdwUserIndex, unsigned int dwFlags,
                                         _XINPUT_KEYSTROKE *pKeystroke);

unsigned int input_update_keyboard()
{
    for (int i = 0; i < 109; i = (int16_t)(i + 1))
    {
        int ticks;
        if (input_globals.key_latches[i])
        {
            ticks = input_globals.key_ticks[i] + 1;
            if (ticks > 255)
                ticks = 255;
        }
        else
        {
            ticks = 0;
        }
        input_globals.key_ticks[i] = ticks;
    }

    input_globals.buffered_key_read_index = 0;
    input_globals.buffered_key_write_index = 0;

    unsigned int user_index = 255;
    _XINPUT_KEYSTROKE keystroke[7];
    unsigned int result;
    for (result = XInputGetKeystrokeEx(&user_index, 2u, keystroke); !result;
         result = XInputGetKeystrokeEx(&user_index, 2u, keystroke))
    {
        uint16_t flags = keystroke[0].Flags;

        unsigned char modifiers;
        if ((flags & 0x10) != 0) /* ctrl */
            modifiers = ((flags & 8) != 0) | (1u << _key_modifier_control_bit);
        else
            modifiers = (flags & 8) != 0;

        key_stroke stroke;
        if ((flags & 0x20) != 0) /* alt */
            stroke.modifier_flags = modifiers | (1u << _key_modifier_alt_bit);
        else
            stroke.modifier_flags = modifiers & ~(1u << _key_modifier_alt_bit);

        unsigned char ascii_code = (unsigned char)keystroke[0].Unicode;
        if (keystroke[0].Unicode < NUMBER_OF_ASCII_CODES)
        {
            if ((uint16_t)ascii_to_key_table[keystroke[0].Unicode] == 0xFFFF)
                ascii_code = (unsigned char)-1;
            stroke.ascii_code = ascii_code;
            stroke.key_code = virtual_to_key_table[keystroke[0].VirtualKey];

            if ((unsigned int)stroke.key_code <= _key_102_oem)
            {
                if ((flags & 2) != 0) /* key up */
                {
                    input_globals.key_latches[stroke.key_code] = 0;
                    if (input_globals.key_ticks[stroke.key_code] > 1u)
                        input_globals.key_ticks[stroke.key_code] = 0;
                }
                else /* key down */
                {
                    if (input_globals.buffered_key_write_index < 64)
                        input_globals.buffered_keys[input_globals.buffered_key_write_index++] = stroke;
                    input_globals.key_latches[stroke.key_code] = 1;
                    if (!input_globals.key_ticks[stroke.key_code])
                        input_globals.key_ticks[stroke.key_code] = 1;
                }
            }
        }
    }

    return result;
}
