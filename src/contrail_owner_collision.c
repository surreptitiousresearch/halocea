/* contrail_owner_collision @ 0x8373FD10 — advance a contrail attached to an object, typically when the
 * owner moves or is being torn down. If the contrail is still emitting (flags short[1] bit0) it appends
 * however many new points the elapsed time calls for (at least one). When the owner is dying the owner
 * reference (contrail+8) is cleared so the contrail fades out detached. Finally the contrail's age
 * accumulator (contrail float[10]) is advanced by dt. Contrail datum stride is 68 bytes. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/contrail_datum.h"
#include "headers/contrail_active_flags.h"
#include "headers/blam_data_globals.h"

extern int16_t contrail_compute_new_point_count(int contrail_index, float dt);
extern void contrail_add_points(int contrail_index, int16_t contrail_point_count, uint8_t force);

void contrail_owner_collision(int contrail_index, uint8_t object_dying, float dt)
{
    contrail_datum *contrail = DATA_ARRAY_ELEMENT(contrail_data, contrail_datum, contrail_index);
    if ( (contrail->flags & (1u << _contrail_active_bit)) != 0 )
    {
        int16_t new_point_count = contrail_compute_new_point_count(contrail_index, dt);
        if ( new_point_count < 1 )
            new_point_count = 1;
        contrail_add_points(contrail_index, new_point_count, 0);
    }
    if ( object_dying )
        contrail->object_index = -1;
    contrail->expired_dt += dt;
}
