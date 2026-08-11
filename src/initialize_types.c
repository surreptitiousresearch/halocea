#include "headers/blam_data_globals.h"

/* DEVIATION: the base symbol was read off IDA's DISPLACEMENT spelling. @0x837A1108 the binary does
 * `addi r11, r11, (gITEM_PLACEMENT_BITS_Z - 0x84182E10)` -- that expression is IDA printing the
 * constant -4, not a reference to gITEM_PLACEMENT_BITS_Z (a 4-byte int at 0x84182E0C, which happens
 * to sit exactly 4 bytes below the real base). r11 comes from
 * `lis/addi _message_delta_global_field_type_list` @0x837A10FC-1100 (0x84182E10), so the 28
 * `stbu r10, 0x18(r11)` @0x837A1110 write byte 0x14 -- `initialized` -- of each 24-byte row of
 * message_delta_global_field_type_list[28], which is what the old comment already described. The
 * old transcription walked 672 bytes off a 4-byte global. Byte-for-byte the same 28 stores. */
void initialize_types(void)
{
    for ( int type_index = 0; type_index < 28; type_index++ )
        message_delta_global_field_type_list[type_index].initialized = 1;
}
