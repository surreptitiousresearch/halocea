#pragma once
/* data_packet_field — one field in a packet definition's wire format (10 bytes). A field list is terminated by
 * a __pack_end field. `type` is a packet_field_type (see packet_field_type.h) selecting the encoder:
 * __pack_short=u16, __pack_long=u32, __pack_int64=u64, __pack_string, __pack_data=variable byte array,
 * __pack_array=variable nested-array (recurses over the following sub-fields), other=raw bytes. */

typedef struct data_packet_field
{
    __int16 type;          /* 0x00 */
    __int16 count;         /* 0x02 — element count / maximum */
    __int16 first_version; /* 0x04 */
    __int16 last_version;  /* 0x06 — 0 = no upper bound */
    __int16 size;          /* 0x08 — decoded byte stride */
} data_packet_field;
