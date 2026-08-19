/* input_abstraction_debounce_auxbutton_key @0x83724DE0 — clears the debounce tick count for the
 * auxbutton mapping entry matching key_code (no-op if key_code isn't one of the 3 mapped aux buttons). */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/blam_data_globals.h"
#include "headers/auxbutton.h"

void input_abstraction_debounce_auxbutton_key(int16_t key_code)
{
    int i = 0;
    int16_t *m = keyboard_auxbutton_mapping;

    while (*m != key_code)
    {
        m++;
        i++;
        if (m >= &keyboard_auxbutton_mapping[NUMBER_OF_AUXCONTROL_BUTTONS])   /* one past the DB-typed __int16[3] map */
            return;
    }

    input_abstraction_globals.auxbutton_ticks[i] = 0;
}
