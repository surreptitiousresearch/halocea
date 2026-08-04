#pragma once
/* data_packet_group_definition — a named group of packet type definitions sharing encode/decode buffers. */

#include "data_packet_group_packet.h"

typedef struct data_packet_group_definition
{
    const char               *name;                        /* 0x00 */
    __int16                   packet_type_count;           /* 0x04 */
    __int16                   packet_class_count;          /* 0x06 */
    int                       maximum_decoded_packet_size; /* 0x08 */
    int                       maximum_encoded_packet_size; /* 0x0C */
    data_packet_group_packet *packets;                     /* 0x10 */
} data_packet_group_definition;             /* 20 bytes */
