#pragma once
/* vibrate_data — final left/right controller motor amplitude pair (4 bytes, DB types_members-confirmed
 * layout). Small enough to be returned by value packed into a single register. */

typedef struct vibrate_data
{
    unsigned __int16 left_frequency;  /* 0x0 */
    unsigned __int16 right_frequency; /* 0x2 */
} vibrate_data; /* 4 bytes */
