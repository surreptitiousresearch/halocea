#pragma once
/* unit_seat — one element of _unit_definition.seats (284 bytes). Full DB types_members layout. */

#include <stdint.h>
#include "real_vector3d.h"
#include "unit_camera.h"
#include "tag_block.h"
#include "tag_reference.h"

typedef struct unit_seat
{
    unsigned int  flags;                     /* 0x000 */
    char          label[32];                 /* 0x004 */
    char          marker_name[32];           /* 0x024 */
    int           unused[8];                 /* 0x044 */
    real_vector3d acceleration_scale;        /* 0x064 */
    int           unused2[3];                /* 0x070 */
    float         yaw_rate;                  /* 0x07C */
    float         pitch_rate;                /* 0x080 */
    unit_camera   camera;                    /* 0x084 */
    tag_block     seat_huds;                 /* 0x0DC */
    int           pad0;                      /* 0x0E8 */
    int16_t       icon_text_index;           /* 0x0EC */
    int16_t       pad;                       /* 0x0EE */
    float         yaw_minimum;               /* 0x0F0 */
    float         yaw_maximum;               /* 0x0F4 */
    tag_reference built_in_actor_reference;  /* 0x0F8 */
    int           unused3[5];                /* 0x108 */
} unit_seat;                                 /* 284 bytes (0x11C) */
