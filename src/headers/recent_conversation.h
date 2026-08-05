#include <stdint.h>
#ifndef RECENT_CONVERSATION_H
#define RECENT_CONVERSATION_H

/* A recently-finished AI conversation, retained so scripts can query its outcome after it ends. */
typedef struct recent_conversation
{
    int16_t         definition_index;        /* 0x00 */
    uint8_t unable_to_begin;         /* 0x02 */
    uint8_t finished_successfully;   /* 0x03 */
    int             finish_time;             /* 0x04 */
    unsigned int    unused[2];               /* 0x08 */
} recent_conversation;                       /* 0x10 */

#endif /* RECENT_CONVERSATION_H */
