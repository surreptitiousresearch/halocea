#include <stdint.h>
#include "headers/ai_information_packet.h"

extern uint8_t reply_filter_close(int original_unit_index, ai_information_packet *communication, int reply_actor_index);
extern uint8_t actor_is_fighting(uint16_t actor_index);

int reply_filter_fighting_close(int original_unit_index, ai_information_packet *communication, int reply_actor_index)
{
    if ( !reply_filter_close(original_unit_index, communication, reply_actor_index) )
        return 0;
    return actor_is_fighting(reply_actor_index) != 0;
}
