/* attachments_delete @ 0x836F05E0 — tear down every attachment an object carries (looping sounds,
 * effects, contrails, particle systems, lights). The model definition's attachment count is at
 * def+320; per attachment slot i a type byte sits at object_data+324+i and the attached datum index at
 * object_data int[83+i] (+332+4*i). Type 255 means an empty slot. The type selects how the attachment
 * is released. Mirrors object_connect_lights' slot layout. */

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
        __int16 i = 0;
        do
        {
            unsigned __int8 attachment_type = ((unsigned char *)object_data)[324 + i];
            if ( attachment_type != _object_attachment_type_none )
            {
                int attachment_index = ((int *)object_data)[i + 83];  /* raw int[] at +332+4*i */
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
                            contrail_owner_collision(((int *)object_data)[i + 83], 1u, 0.0f);
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
            i = (__int16)(i + 1);
        }
        while ( i < object_definition->attachments.count );
    }
}
