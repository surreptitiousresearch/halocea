#pragma once
/* pursuit_location — a remembered firing/pursuit spot an actor is heading toward. Layout from the database. */

#include "real_point3d.h"

typedef struct pursuit_location
{
    __int16       type;                  /* 0x00 */
    __int16       firing_position_index; /* 0x02 */
    __int16       cluster_index;         /* 0x04 */
    unsigned char _pad0[2]; /* db-verified padding */
    int           surface_index;         /* 0x08 */
    real_point3d  position;              /* 0x0C */
} pursuit_location; /* 24 bytes */
