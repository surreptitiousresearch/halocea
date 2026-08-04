/* ai_communication_packet_new @0x837CBCD8 — initialise an AI information/communication packet: zero the whole
 * 32-byte payload, then set the target unit, communication type, dialogue type and damage category fields to
 * their "none" sentinel (-1). */

#include "headers/ai_information_packet.h"

void ai_communication_packet_new(ai_information_packet *information)
{
    int *words = (int *)information;
    for ( int i = 0; i < 8; ++i )
        words[i] = 0;
    information->target_unit_index = -1;
    information->communication_type = -1;
    information->dialogue_type_index = -1;
    information->damage_category = -1;
}
