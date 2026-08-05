#ifndef OBEY_STATE_DATA_H
#define OBEY_STATE_DATA_H

#include <stdint.h>
#include "obey_individual_simple_control.h"
#include "obey_individual_complex_control.h"

typedef struct obey_state_data
{
    int16_t                         command_list_index;    /* 0x00 */
    uint8_t                 initiative;            /* 0x02 */
    uint8_t                 allow_looking;         /* 0x03 */
    uint8_t                 allow_communication;   /* 0x04 */
    uint8_t                 finished;              /* 0x05 */
    char                            pad_6[2];              /* 0x06 */
    obey_individual_simple_control  simple_control;        /* 0x08 */
    obey_individual_complex_control complex_control;       /* 0x2C */
} obey_state_data;                                         /* 0x84 */

#endif /* OBEY_STATE_DATA_H */
