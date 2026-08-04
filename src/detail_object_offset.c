#include "headers/blam_data_globals.h"

void detail_object_offset(float offset)
{
    fudge_vector = 1;
    final_offset = offset;
    fudge_offset = offset - fudge_offset;
}
