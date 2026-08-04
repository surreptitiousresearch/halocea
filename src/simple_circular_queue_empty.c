/* simple_circular_queue_empty @0x837AB288 — reset a simple circular queue to empty by zeroing its read and
 * write positions. */

#include "headers/simple_circular_queue.h"

void simple_circular_queue_empty(simple_circular_queue *queue)
{
    queue->read_position = 0;
    queue->write_position = 0;
}
