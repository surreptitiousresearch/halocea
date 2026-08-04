#include "headers/blam_data_globals.h"

/* DEVIATION: decompiler shows loop over 28 field_type_definition structs at 24-byte stride;
   sets initialized flag (1 byte past the 6th int of each struct) to 1 */
void initialize_types(void)
{
    int *v = &gITEM_PLACEMENT_BITS_Z;
    int n = 28;
    do {
        v += 6;
        *(char *)v = 1;
        --n;
    } while (n);
}
