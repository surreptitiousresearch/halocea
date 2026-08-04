/* object_dead_recon_position @0x836ECC94 — dead-reckon an object's current position from its last
 * networked update. Requires the object's three prediction-validity flags to be set, a nonzero elapsed
 * time since the last update (ms), and a meaningful speed (> 0.05). The predicted position is
 * last_server_position advanced along the live translational_velocity by elapsed_ms * 0.001 * 30 * speed.
 * Returns 1 when a prediction was written, 0 otherwise.
 *
 * DEVIATION: the decompiler packs the elapsed-ms int and the stored position.y into the two halves of
 * one 64-bit temp (register reuse), making `(float)v6` look like a 64-bit conversion. They are
 * independent: the time factor uses only the elapsed ms; position.y is read separately. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern uint32_t system_milliseconds(void);

uint8_t object_dead_recon_position(int object_index, real_point3d *predicted_position)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if (object->object.is_server_position_valid != 1)
        return 0;
    if (object->object.is_server_translational_velocity_valid != 1)
        return 0;
    if (object->object.is_update_timestamp_valid != 1)
        return 0;

    int last_update_ms = object->object.last_update_timestamp;
    unsigned int now = system_milliseconds();
    if (now == (unsigned int)last_update_ms)
        return 0;

    float vx = object->object.translational_velocity.n[0];
    float vy = object->object.translational_velocity.n[1];
    float vz = object->object.translational_velocity.n[2];
    float speed = __fsqrts(vz * vz + (vy * vy + vx * vx));
    if (speed <= 0.050000001f)
        return 0;

    float factor = (float)(now - (unsigned int)last_update_ms) * 0.001f * 30.0f * speed;
    predicted_position->n[0] = vx * factor + object->object.last_server_position.n[0];
    predicted_position->n[1] = vy * factor + object->object.last_server_position.n[1];
    predicted_position->n[2] = vz * factor + object->object.last_server_position.n[2];
    return 1;
}
