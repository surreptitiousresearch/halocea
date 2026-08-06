/* attachments_delete @ 0x836F05E0 — tear down every attachment an object carries (looping sounds,
 * effects, contrails, particle systems, lights). The definition's attachments block count drives the
 * loop; per attachment slot i the type byte is object.attachment_types[i] and the attached datum index
 * object.attachment_indices[i]. Type 255 — the truncated -1 attachments_new stores — means an empty
 * slot. The type selects how the attachment is released. Mirrors object_connect_lights' slot layout. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/object_attachment_type.h"
#include "headers/blam_data_globals.h"


extern void game_looping_sound_delete(int looping_sound_index);
extern void effect_delete(int effect_index);
extern void object_compute_node_matrices(int object_index);
extern void contrail_owner_collision(uint16_t contrail_index, uint8_t object_dying, float dt);
extern void particle_system_orphan(uint16_t system_index);
extern void light_delete(int light_index);

void attachments_delete(int object_index)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *object_definition = TAG_GET(_object_definition, object_data->definition_index);

    if ( object_definition->attachments.count > 0 )
    {
        int16_t i = 0;
        do
        {
            /* DEVIATION: types_members and headers_ref both spell _object_datum.attachment_types as
             * char[8], but the binary reads the slot zero-extended and compares it unsigned throughout
             * (lbz + cmplwi 0xFF @0x836F064C/0x836F0650, cmplwi 4 @0x836F0670) — a signed read would
             * make the 255 test always true and let 0x80..0xFE pass the <= 4 test. Read it as a byte. */
            uint8_t attachment_type = object_data->object.attachment_types[i];
            if ( attachment_type != _object_attachment_type_none )
            {
                int attachment_index = object_data->object.attachment_indices[i];
                if ( attachment_index != -1 && attachment_type <= _object_attachment_type_particle_system )
                {
                    switch ( attachment_type )
                    {
                        case _object_attachment_type_looping_sound:
                            game_looping_sound_delete(attachment_index);
                            break;
                        case _object_attachment_type_effect:
                            effect_delete(attachment_index);
                            break;
                        case _object_attachment_type_contrail:
                            object_compute_node_matrices(object_index);
                            /* re-read, not attachment_index: the binary reloads the slot at 0x836F06B4
                             * after the call, which no compiler may rematerialize across an opaque call */
                            contrail_owner_collision(object_data->object.attachment_indices[i], 1u, 0.0f);
                            break;
                        default:   /* particle_system (4) or light (0) */
                            if ( attachment_type )
                                particle_system_orphan(attachment_index);
                            else
                                light_delete(attachment_index);
                            break;
                    }
                }
            }
            i = (int16_t)(i + 1);
        }
        while ( i < object_definition->attachments.count );
    }
}
