#include <stdint.h>
#ifndef OBEY_INDIVIDUAL_JUMP_CONTROL_H
#define OBEY_INDIVIDUAL_JUMP_CONTROL_H

typedef struct obey_individual_jump_control
{
    int16_t delay_ticks;              /* 0x00 */
    char    _pad0[2];                 /* 0x02 */
    float   target_horizontal_vel;    /* 0x04 */
    float   target_vertical_vel;      /* 0x08 */
} obey_individual_jump_control;       /* 0x0C */

#endif /* OBEY_INDIVIDUAL_JUMP_CONTROL_H */
