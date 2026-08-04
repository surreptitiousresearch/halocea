#pragma once
/* data_packet_definition — describes one packet type's wire format: its ordered field list, default version,
 * and a lazily-set initialized flag. Full DB layout (17+ bytes). */

typedef struct data_packet_field data_packet_field;

typedef struct data_packet_definition
{
    char              *name;             /* 0x00 */
    unsigned int       flags;            /* 0x04 */
    __int16            size;             /* 0x08 */
    __int16            version;          /* 0x0A */
    data_packet_field *fields;           /* 0x0C */
    unsigned __int8    initialized_flag; /* 0x10 — set after first verify */
    unsigned __int8    pad_0x11[3];      /* 0x11 — padding to align(4) */
} data_packet_definition; /* 20 bytes */
