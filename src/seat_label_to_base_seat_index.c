#include <stdint.h>
#include "headers/blam_data_globals.h"
/* seat_label_to_base_seat_index @0x836CC050 — map a seat label string to its index in the fixed table of
 * base seat labels (6 entries), case-insensitively. Returns -1 if the label is not one of the base seats. */

extern int stricmp(const char *a, const char *b);

int16_t seat_label_to_base_seat_index(const char *seat_label)
{
    for ( int16_t index = 0; index < 6; ++index )
    {
        if ( !stricmp(seat_label, base_seat_labels[index]) )
            return index;
    }
    return -1;
}
