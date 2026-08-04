/* debug_keys_update @ 0x836EAC90 — poll every bound debug key. The required modifier
 * combination is derived from _key_shift/_key_control; a key fires only when its scancode is down and
 * its modifier combo matches. Non-toggle keys drive their bound bool directly; toggle keys
 * flip on release. The per-key edge callback receives 1 on press, 0 on release. */
#include <stdint.h>
#include "headers/debug_key.h"
#include "headers/key_code.h"

extern uint8_t input_key_is_down(int16_t key_code);

void debug_keys_update(void)
{
    unsigned char mod_a = input_key_is_down(_key_shift);
    unsigned char mod_b = input_key_is_down(_key_control);
    /* modifier-combo table indexed by debug_key.modifier (0=none,1=A,2=B,3=A+B) */
    char combo[4];
    int i;
    unsigned int bit;

    combo[0] = !mod_a && !mod_b;
    combo[1] =  mod_a && !mod_b;
    combo[2] = !mod_a &&  mod_b;
    combo[3] =  mod_a &&  mod_b;

    if ( !global_debug_key_list[0].name )
        return;

    i = 0;
    bit = 1;
    do
    {
        debug_key *key = &global_debug_key_list[i];
        unsigned char fired = input_key_is_down(key->key_code) && combo[key->modifier];
        int word = i >> 5;
        int was_down = global_debug_key_down[word] & bit;

        if ( !key->toggle_variable && key->variable )
            *key->variable = fired;

        if ( was_down )
        {
            if ( !fired )
            {
                global_debug_key_down[word] &= ~bit;
                if ( key->toggle_variable && key->variable )
                    *key->variable = *key->variable == 0;
                if ( key->function )
                    key->function(0);   /* release edge */
            }
        }
        else if ( fired )
        {
            global_debug_key_down[word] |= bit;
            if ( key->function )
                key->function(1);       /* press edge */
        }

        ++i;
        bit = (bit << 1) | (bit >> 31);  /* rotate-left, matching __ROL4__ */
    }
    while ( global_debug_key_list[i].name );
}
