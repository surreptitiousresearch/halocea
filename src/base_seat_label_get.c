#include <stdint.h>
#include "headers/blam_data_globals.h"

const char * base_seat_label_get(int16_t base_seat_index)
{
    return base_seat_labels[base_seat_index];
}
