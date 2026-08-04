#include <stdint.h>
#include "headers/blam_data_globals.h"
/* set_language_code @0x83814D70 — set the active language (0..5), defaulting to 0 (English) for out-of-range
 * codes. */


void set_language_code(int16_t language_code)
{
    if ( (unsigned int)language_code < 6 )
        global_language_code = language_code;
    else
        global_language_code = 0;
}
