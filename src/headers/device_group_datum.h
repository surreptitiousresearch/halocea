#pragma once
/* device_group_datum — a runtime device-group instance (DB types_members-confirmed, 8 bytes). */

typedef struct device_group_datum
{
    __int16          identifier;    /* 0x0 */
    unsigned __int16 flags;         /* 0x2 */
    float            desired_value; /* 0x4 */
} device_group_datum; /* 8 bytes */
