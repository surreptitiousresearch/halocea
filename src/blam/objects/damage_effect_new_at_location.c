/* damage_effect_new_at_location @0x836B21B8 — spawn a damage effect (e.g. an impact/scorch) at a point on
 * an object, supplying the effect's named coordinate frame so its emitters can orient to the hit. Builds
 * five marker "forward" vectors keyed by name — normal, incident, negative incident, reflection, gravity —
 * and five marker points (all at the hit position), then creates the effect attached to the object node
 * (if object/node valid) or unattached.
 *
 * The incident vector is the normalised damage direction (falling back to global_forward3d if degenerate).
 * The surface normal is the caller's normal, or the direction from the object origin to the hit point, or
 * the object's own forward vector. The reflection is incident reflected about the normal.
 *
 * Deviation: the markers block is laid out contiguously on the stack as the decompiler's v38/v39/v42/v43/
 * v44 — reproduced here as marker_forwards[5] in that exact memory order (the name/vector pairing is
 * positional). The two from_markers calls' trailing color and vector-field args are NULL (r7=0, verified
 * at 0x836B2490). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector.h"
#include "headers/blam_data_globals.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/ppc_intrinsics.h"


extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern float normalize3d(real_vector3d *v);
extern real_vector3d *reflect_vector3d(const real_vector3d *incident, const real_vector3d *normal, real_vector3d *reflection);
extern int effect_new_unattached_from_markers(int definition_index, int owner_object_index, const real_vector3d *translational_velocity, int16_t marker_count, const char **marker_names, real_point3d *marker_points, real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field, uint8_t deterministic);
extern int effect_new_attached_from_markers(int definition_index, int owner_object_index, int object_index, int16_t node_index, int16_t marker_count, const char **marker_names, const real_point3d *marker_points, const real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);

void damage_effect_new_at_location(int effect_definition_index, int object_index, int16_t node_index,
                                   const real_point3d *position, const real_vector3d *direction,
                                   const real_vector3d *normal)
{
    const char *marker_names[NUMBER_OF_EFFECT_MARKERS];
    marker_names[_effect_vector_normal] = "normal";
    marker_names[_effect_vector_incident] = "incident";
    marker_names[_effect_vector_negative_incident] = "negative incident";
    marker_names[_effect_vector_reflected] = "reflection";
    marker_names[_effect_vector_gravity] = "gravity";

    /* incident = normalised damage direction (fall back to global_forward3d when degenerate) */
    real_vector3d incident;
    incident.n[0] = direction->n[0];
    incident.n[1] = direction->n[1];
    incident.n[2] = direction->n[2];
    float magnitude = __fsqrts(direction->n[2] * direction->n[2]
                               + (direction->n[1] * direction->n[1] + direction->n[0] * direction->n[0]));
    if ( __fabs(magnitude) < 0.000099999997f )
    {
        incident = *global_forward3d;
    }
    else
    {
        incident.n[0] = (1.0f / magnitude) * direction->n[0];
        incident.n[1] = direction->n[1] * (1.0f / magnitude);
        incident.n[2] = (1.0f / magnitude) * direction->n[2];
        if ( magnitude == 0.0f )
            incident = *global_forward3d;
    }

    /* surface normal: caller's, else origin->hit direction, else the object's forward vector */
    real_vector3d surface_normal;
    if ( normal )
    {
        surface_normal = *normal;
    }
    else
    {
        real_point3d origin;
        object_get_origin(object_index, &origin);
        surface_normal.n[0] = position->n[0] - origin.n[0];
        surface_normal.n[1] = position->n[1] - origin.n[1];
        surface_normal.n[2] = position->n[2] - origin.n[2];
        if ( normalize3d(&surface_normal) == 0.0f )
        {
            object_datum *object_data =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
            surface_normal.n[0] = object_data->object.forward.n[0];
            surface_normal.n[1] = object_data->object.forward.n[1];
            surface_normal.n[2] = object_data->object.forward.n[2];
        }
    }

    real_vector3d reflection;
    reflect_vector3d(&incident, &surface_normal, &reflection);

    /* marker forward vectors, in memory order matching marker_names positionally */
    real_vector3d marker_forwards[NUMBER_OF_EFFECT_MARKERS];
    marker_forwards[_effect_vector_normal] = surface_normal;                                   /* normal */
    marker_forwards[_effect_vector_incident].n[0] = incident.n[0] * -1.0f;                       /* (named "incident") */
    marker_forwards[_effect_vector_incident].n[1] = incident.n[1] * -1.0f;
    marker_forwards[_effect_vector_incident].n[2] = incident.n[2] * -1.0f;
    marker_forwards[_effect_vector_negative_incident] = incident;                                         /* (named "negative incident") */
    marker_forwards[_effect_vector_reflected] = reflection;                                       /* reflection */
    marker_forwards[_effect_vector_gravity] = *global_down3d;                                   /* gravity */

    real_point3d marker_points[NUMBER_OF_EFFECT_MARKERS];
    for ( int i = 0; i < NUMBER_OF_EFFECT_MARKERS; ++i )
        marker_points[i] = *position;

    if ( object_index == -1 || node_index == -1 )
        effect_new_unattached_from_markers(effect_definition_index, object_index, global_zero_vector3d, NUMBER_OF_EFFECT_MARKERS,
                                           marker_names, marker_points, marker_forwards, 1.0, 0.0, 0, 0, 0);
    else
        effect_new_attached_from_markers(effect_definition_index, object_index, object_index, node_index, NUMBER_OF_EFFECT_MARKERS,
                                         marker_names, marker_points, marker_forwards, 1.0, 0.0, 0, 0);
}
