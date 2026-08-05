#ifndef SWARM_WANDER_CONTROL_H
#define SWARM_WANDER_CONTROL_H

#include <stdint.h>
#include "real_vector3d.h"

typedef struct swarm_wander_control
{
    uint8_t pause_ticks;    /* 0x00 */
    uint8_t move_ticks;     /* 0x01 */
    uint8_t pad[2];         /* 0x02 */
    real_vector3d   vector;         /* 0x04 */
    float           angle;          /* 0x10 */
} swarm_wander_control;             /* 0x14 */

#endif /* SWARM_WANDER_CONTROL_H */
