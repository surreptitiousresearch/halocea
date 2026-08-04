#ifndef OBEY_STATE_DATA_H
#define OBEY_STATE_DATA_H

#include "obey_individual_simple_control.h"
#include "obey_individual_complex_control.h"

typedef struct obey_state_data
{
    __int16                         command_list_index;    /* 0x00 */
    unsigned __int8                 initiative;            /* 0x02 */
    unsigned __int8                 allow_looking;         /* 0x03 */
    unsigned __int8                 allow_communication;   /* 0x04 */
    unsigned __int8                 finished;              /* 0x05 */
    char                            pad_6[2];              /* 0x06 */
    obey_individual_simple_control  simple_control;        /* 0x08 */
    obey_individual_complex_control complex_control;       /* 0x2C */
} obey_state_data;                                         /* 0x84 */

#endif /* OBEY_STATE_DATA_H */
