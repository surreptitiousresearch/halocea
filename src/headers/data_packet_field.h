#pragma once
#include <stdint.h>
/* data_packet_field — one field in a packet definition's wire format (10 bytes). A field list is terminated by
 * a __pack_end field. `type` is a packet_field_type (see packet_field_type.h) selecting the encoder:
 * __pack_short=u16, __pack_long=u32, __pack_int64=u64, __pack_string, __pack_data=variable byte array,
 * __pack_array=variable nested-array (recurses over the following sub-fields), other=raw bytes. */

typedef struct data_packet_field
{
    int16_t type;          /* 0x00 */
    int16_t count;         /* 0x02 — element count / maximum */
    int16_t first_version; /* 0x04 */
    int16_t last_version;  /* 0x06 — 0 = no upper bound */
    int16_t size;          /* 0x08 — decoded byte stride */
} data_packet_field;
