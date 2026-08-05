#pragma once
#include <stdint.h>
/* data_packet_definition — describes one packet type's wire format: its ordered field list, default version,
 * and a lazily-set initialized flag. Full DB layout (17+ bytes). */

typedef struct data_packet_field data_packet_field;

typedef struct data_packet_definition
{
    char              *name;             /* 0x00 */
    unsigned int       flags;            /* 0x04 */
    int16_t            size;             /* 0x08 */
    int16_t            version;          /* 0x0A */
    data_packet_field *fields;           /* 0x0C */
    uint8_t    initialized_flag; /* 0x10 — set after first verify */
    uint8_t    pad_0x11[3];      /* 0x11 — padding to align(4) */
} data_packet_definition; /* 20 bytes */
