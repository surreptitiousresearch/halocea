/* unit_get_camera_position @ 0x836C9128 — compute the eyepoint a unit's camera should use. For a free
 * (unparented) unit: a biped (object type 0, not dead/destroyed) uses biped_get_sight_position; otherwise
 * the unit uses either its own "head" marker, or — when it has a gunner child (unit.gunner_object_index) —
 * that child's seat marker (unit definition seats block, unit_seat.marker_name, seat index from the child's
 * unit.parent_seat_index). For a seated/parented unit it starts at the parent object.position and, when the
 * parent is a biped/vehicle (type 0 or 1) and the unit occupies a seat with a defined camera marker
 * (unit_seat.camera.marker_name), reads that marker instead. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_marker.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"


extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void biped_get_sight_position(int biped_index, int16_t estimate_mode, const real_point3d *estimated_body_position, real_vector3d *desired_facing, const real_vector3d *desired_gun_offset, real_point3d *sight_position);

void unit_get_camera_position(int unit_index, real_point3d *camera_position)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    object_marker marker;

    if ( unit->object.parent_object_index == -1 )
    {
        if ( (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 || unit->object.type )
        {
            if ( unit->unit.gunner_object_index == -1 )
            {
                object_get_marker_by_name(unit_index, "head", &marker, 1);
                camera_position->n[0] = marker.matrix.n[3][0];
                camera_position->n[1] = marker.matrix.n[3][1];
                camera_position->n[2] = marker.matrix.n[3][2];
            }
            else
            {
                int child = unit->unit.gunner_object_index;
                unit_datum *child_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum);
                int seat_index = child_data->unit.parent_seat_index;
                unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);
                const char *marker_name =
                    ((const unit_seat *)definition->unit.seats.address)[seat_index].marker_name;
                object_get_marker_by_name(unit_index, marker_name, &marker, 1);
                camera_position->n[0] = marker.matrix.n[3][0];
                camera_position->n[1] = marker.matrix.n[3][1];
                camera_position->n[2] = marker.matrix.n[3][2];
            }
        }
        else
        {
            /* this branch is only reached when object.type == object_type_biped (0), so desired_facing is NULL
             * (the decompiler passed object.type, which is provably 0 here, through the arg register) */
            biped_get_sight_position(unit_index, 0, nullptr,
                nullptr, nullptr, camera_position);
        }
    }
    else
    {
        int parent_object = unit->object.parent_object_index;
        unit_datum *parent = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_object)->datum);
        *camera_position = parent->object.position;
        int16_t parent_type = parent->object.type;
        if ( ((1 << parent_type) & object_mask_unit) != 0 )
        {
            int seat_index = unit->unit.parent_seat_index;
            if ( seat_index != -1 )
            {
                unit_definition *parent_definition = TAG_GET(unit_definition, parent->definition_index);
                const unit_seat *seat = (const unit_seat *)parent_definition->unit.seats.address + seat_index;
                if ( parent_type != 1 || seat->camera.marker_name[0] )
                {
                    object_get_marker_by_name(unit->object.parent_object_index, seat->camera.marker_name, &marker, 1);
                    camera_position->n[0] = marker.matrix.n[3][0];
                    camera_position->n[1] = marker.matrix.n[3][1];
                    camera_position->n[2] = marker.matrix.n[3][2];
                }
            }
        }
    }
}
