#ifndef OBEY_INDIVIDUAL_SIMPLE_CONTROL_H
#define OBEY_INDIVIDUAL_SIMPLE_CONTROL_H

#include <stdint.h>
#include "obey_individual_directmovement_control.h"
#include "obey_individual_jump_control.h"

/* DB anonymous union _3369FF960A473C765539227E34924144 (28 bytes) — obey_individual_simple_control
 * member ___u5 ($ in identifiers is an MSVC extension). */
typedef union _3369FF960A473C765539227E34924144
{
    obey_individual_directmovement_control directmovement; /* 0x00 */
    obey_individual_jump_control           jump;           /* 0x00 */
} _3369FF960A473C765539227E34924144;

typedef struct obey_individual_simple_control
{
    uint8_t current_command_index;    /* 0x00 */
    uint8_t loop_counter;             /* 0x01 */
    int16_t         pause_timer;              /* 0x02 */
    uint8_t metadata_flags;           /* 0x04 */
    uint8_t simple_control_flags;     /* 0x05 */
    uint8_t _pad06[2];                /* 0x06 */
    union _3369FF960A473C765539227E34924144 ___u5; /* 0x08 */
} obey_individual_simple_control;             /* 0x24 */

#endif /* OBEY_INDIVIDUAL_SIMPLE_CONTROL_H */
