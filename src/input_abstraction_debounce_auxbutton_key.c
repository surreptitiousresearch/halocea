/* input_abstraction_debounce_auxbutton_key @0x83724DE0 — clears the debounce tick count for the
 * auxbutton mapping entry matching key_code (no-op if key_code isn't one of the 3 mapped aux buttons). */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/blam_data_globals.h"

extern int16_t keyboard_auxbutton_mapping_end;   /* one-past-end sentinel of keyboard_auxbutton_mapping */

void input_abstraction_debounce_auxbutton_key(int16_t key_code)
{
    int i = 0;
    int16_t *m = keyboard_auxbutton_mapping;

    while (*m != key_code)
    {
        m++;
        i++;
        if ((int)m >= (int)&keyboard_auxbutton_mapping_end)
            return;
    }

    input_abstraction_globals.auxbutton_ticks[i] = 0;
}
