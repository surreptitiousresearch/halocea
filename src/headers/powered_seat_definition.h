#pragma once
/* powered_seat_definition — one element of unit_definition_unit.powered_seats (68 bytes), per an
 * authoritative structure dump (C:\halo\headers_ref\powered_seat_definition.h). Its powerup/powerdown
 * times drive the per-seat integrated-light ramp rate in unit_update.c. */

typedef struct powered_seat_definition
{
    int   pad;              /* 0x00 */
    float powerup_time;      /* 0x04 */
    float powerdown_time;     /* 0x08 */
    int   unused[14];          /* 0x0C */
} powered_seat_definition;     /* 0x44 = 68 bytes */
