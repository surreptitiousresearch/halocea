#pragma once
/* data_packet_group_packet — one packet-class entry within a data_packet_group_definition. */

typedef struct data_packet_definition data_packet_definition;

typedef struct data_packet_group_packet
{
    __int16                 packet_class; /* 0x00 */
    char                    _pad2[2];     /* 0x02 */
    data_packet_definition *definition;   /* 0x04 */
} data_packet_group_packet;              /* 8 bytes */
