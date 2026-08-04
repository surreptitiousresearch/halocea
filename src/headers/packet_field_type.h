#pragma once
/* packet_field_type — wire field-type selector stored in data_packet_field.type. Drives the encode/decode/
 * verify switch in the data_packet codec. DB: anonymous enum _D6131E949B1637B26AF4C96FEED7233B
 * (NUMBER_OF_PACKET_FIELD_TYPES = 10). A field list is terminated by a __pack_end field. */

typedef enum packet_field_type
{
    __pack_pad = 0,          /* raw byte block, no version gating payload */
    __pack_char = 1,         /* raw bytes */
    __pack_short = 2,        /* u16[count] */
    __pack_long = 3,         /* u32[count] */
    __pack_int64 = 4,        /* u64[count] */
    __pack_string = 5,       /* null-terminated string, max `count` */
    __pack_data = 6,         /* variable byte array, 2-byte length prefix */
    __pack_array = 7,        /* variable nested array, recurses over the following sub-fields */
    __pack_fixed_data = 8,   /* raw bytes */
    __pack_end = 9,          /* field-list terminator */
    NUMBER_OF_PACKET_FIELD_TYPES = 10
} packet_field_type;
