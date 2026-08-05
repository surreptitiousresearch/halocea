#pragma once
#include <stdint.h>
/* device_group_datum — a runtime device-group instance (DB types_members-confirmed, 8 bytes). */

typedef struct device_group_datum
{
    int16_t          identifier;    /* 0x0 */
    uint16_t flags;         /* 0x2 */
    float            desired_value; /* 0x4 */
} device_group_datum; /* 8 bytes */
