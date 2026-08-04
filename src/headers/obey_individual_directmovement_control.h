#ifndef OBEY_INDIVIDUAL_DIRECTMOVEMENT_CONTROL_H
#define OBEY_INDIVIDUAL_DIRECTMOVEMENT_CONTROL_H

#include "real_vector3d.h"
#include "real_point3d.h"

typedef struct obey_individual_directmovement_control
{
    __int16       facing;            /* 0x00 */
    unsigned char pad_2[2];          /* 0x02 */
    real_vector3d vector;            /* 0x04 */
    real_point3d  start_position;    /* 0x10 */
} obey_individual_directmovement_control;   /* 0x1C */

#endif /* OBEY_INDIVIDUAL_DIRECTMOVEMENT_CONTROL_H */
