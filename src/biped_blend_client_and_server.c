/* biped_blend_client_and_server @0x837AC460 — reconcile a network biped's current position
 * with the last position received from the server. The last server position (object.last_server_position,
 * +28..+36) and current position (object.position, +92..+100) are compared: if they differ by more than
 * 5 units, or prediction is disabled, the biped is snapped straight to the last server position. Otherwise
 * the current position is interpolated halfway toward the dead-reckoned predicted position (or the last
 * server position if none is available), and that blended point is used — unless it is NaN or wildly out
 * of range (+/-5000), in which case the last server position is used. Afterwards, if the (now updated)
 * current position still sits more than 2 units from where it started, the biped is snapped to the last
 * server position.
 *
 * DEVIATION: points_interpolate's output pointer is r6 (&blended); the decompiler's v7 is a phantom
 * from the FPR-shadow of the float t (0.5). Confirmed from disasm (addi r6,r1,var_50).
 * NOTE: an earlier comment called +28..36 the "client" position and +92..100 the "server" position;
 * the DB names them object.last_server_position and object.position respectively — corrected. */

#include <stdint.h>
#include "headers/biped_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern uint8_t object_dead_recon_position(int object_index, real_point3d *predicted_position);
extern void points_interpolate(const real_point3d *a, const real_point3d *b, float t, real_point3d *result);
extern void gearbox_object_translate(int object_index, const real_point3d *new_position);

static int point_is_valid(const real_point3d *p)
{
    for (int i = 0; i < 3; ++i)
    {
        float v = p->n[i];
        if (v != v || v < -5000.0f || v > 5000.0f) /* v != v == isnan */
            return 0;
    }
    return 1;
}

void biped_blend_client_and_server(int biped_index)
{
    biped_datum *biped = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;

    real_point3d current_position;
    current_position.n[0] = biped->object.position.n[0];
    current_position.n[1] = biped->object.position.n[1];
    current_position.n[2] = biped->object.position.n[2];

    real_point3d server_position;
    server_position.n[0] = biped->object.last_server_position.n[0];
    server_position.n[1] = biped->object.last_server_position.n[1];
    server_position.n[2] = biped->object.last_server_position.n[2];

    float dx = server_position.n[0] - current_position.n[0];
    float dy = server_position.n[1] - current_position.n[1];
    float dz = server_position.n[2] - current_position.n[2];

    const real_point3d *destination;
    if (__fsqrts(dx * dx + (dz * dz + dy * dy)) > 5.0f || !object_prediction)
    {
        destination = &server_position;
    }
    else
    {
        real_point3d predicted_position;
        if (!object_dead_recon_position(biped_index, &predicted_position))
            predicted_position = server_position;

        real_point3d blended;
        points_interpolate(&current_position, &predicted_position, 0.5f, &blended);

        destination = point_is_valid(&blended) ? &blended : &server_position;
    }

    gearbox_object_translate(biped_index, destination);

    float ex = biped->object.position.n[0] - current_position.n[0];
    float ey = biped->object.position.n[1] - current_position.n[1];
    float ez = biped->object.position.n[2] - current_position.n[2];
    if (__fsqrts(ex * ex + (ez * ez + ey * ey)) > 2.0f)
        gearbox_object_translate(biped_index, &server_position);
}
