/* observer_clear @0x8370E758 — reset a local player's observer (camera) record to a neutral default: identity
 * orientation (world forward/up), the default ~50-degree field of view, the result viewpoint placed at the
 * origin with an invalid BSP location and zero velocity, and a zeroed last_command seeded with the same
 * orientation/FOV. Stamps the header/trailer signatures, marks the record updated for this frame, and clears
 * the first-command flag.
 *
 * DEVIATION: the decompiler split the up-vector copy into a doubleword store plus a trailing scalar (it views
 * the camera-state block through its flat positions[14] overlay); expressed here as the equivalent vector
 * assignment to the named `up` member. The field-of-view constant 0.87266463 is ~50 degrees in radians. */

#include <string.h>
#include "headers/observer.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


void observer_clear(observer *observer)
{
    observer->forward = *global_forward3d;
    observer->up = *global_up3d;
    observer->field_of_view = 0.87266463f;

    observer->result.position = *global_origin3d;
    observer->result.location.cluster_index = -1;
    observer->result.location.leaf_index = -1;
    observer->result.velocity = *global_zero_vector3d;
    observer->result.forward = *global_forward3d;
    observer->result.up = *global_up3d;
    observer->result.field_of_view = 0.87266463f;

    memset(&observer->last_command, 0, sizeof(observer->last_command));
    observer->last_command.field_of_view = observer->field_of_view;
    observer->last_command.forward = observer->forward;
    observer->last_command.up = observer->up;

    observer->trailer_signature = 0x72616421; /* 'rad!' */
    observer->header_signature = 0x72616421;  /* 'rad!' */
    observer->updated_for_frame = 1;
    observer->first_command = 0;
}
