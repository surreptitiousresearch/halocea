/* action_queue_shutdown @0x837AAFB8 — free an action queue's entry bank and null the pointer. */

#include "headers/action_queue.h"

extern void dlFree(void *ptr);

void action_queue_shutdown(action_queue *queue)
{
    dlFree(queue->entry_bank);
    queue->entry_bank = 0;
}
