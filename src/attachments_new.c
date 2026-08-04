/* attachments_new @0x836EDD20 — instantiate every attachment declared by a new object's definition.
 * Walks the definition's attachment references, dispatches by the attachment's tag group to the matching
 * subsystem creator, and records the attachment type byte and resulting datum index per slot.
 *
 * Group tags (FourCC): 'loop'=looping sound (1), 'effe'=effect (2), 'cont'=contrail (3),
 * 'part'=particle system (4), 'ligh'=light (0). Light and looping-sound success set object flags
 * 0x100 / 0x400. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_attachment_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/object_flags.h"
#include "headers/object_attachment_type.h"
#include "headers/blam_data_globals.h"


extern int game_looping_sound_new(int object_index, int definition_index, const char *marker_name, int16_t function_index);
extern int effect_new_looping(int definition_index, int object_index, int16_t scale_a_function_index, int16_t scale_b_function_index, int16_t change_color_index);
extern int contrail_new(int definition_index, int object_index, int16_t attachment_index);
extern int particle_system_new_attached(int definition_index, int object_index, int16_t attachment_index);
extern int light_new(int definition_index, int object_index, int16_t object_attachment_index, int16_t object_function_index, int16_t object_change_color_index);

void attachments_new(int object_index)
{
    __int16 attachment_index = 0;
    object_datum *object_data =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *object_definition =
        TAG_GET(_object_definition, object_data->definition_index);

    if ( object_definition->attachments.count <= 0 )
        return;

    int slot = 0;
    do
    {
        __int16 attachment_type = -1;
        int datum = -1;
        object_attachment_definition *attachment_ref =
            &((object_attachment_definition *)object_definition->attachments.address)[slot];

        if ( attachment_ref->type.index != -1 )
        {
            unsigned int group_tag = attachment_ref->type.group_tag;
            if ( group_tag > 0x6C696768u )                        /* > 'ligh' */
            {
                if ( group_tag == 0x6C6F6F70u )                   /* 'loop' */
                    attachment_type = _object_attachment_type_looping_sound;
                else if ( group_tag == 0x70617274u )              /* 'part' */
                    attachment_type = _object_attachment_type_particle_system;
            }
            else if ( group_tag == 0x6C696768u )                  /* 'ligh' */
                attachment_type = _object_attachment_type_light;
            else if ( group_tag == 0x636F6E74u )                  /* 'cont' */
                attachment_type = _object_attachment_type_contrail;
            else if ( group_tag == 0x65666665u )                  /* 'effe' */
                attachment_type = _object_attachment_type_effect;
        }

        if ( (unsigned int)attachment_type < NUMBER_OF_OBJECT_ATTACHMENT_TYPES )
        {
            if ( attachment_type == _object_attachment_type_looping_sound )
            {
                datum = game_looping_sound_new(object_index, attachment_ref->type.index,
                    attachment_ref->marker_name, attachment_ref->primary_scale_function_reference - 1);
                if ( datum != -1 )
                    object_data->object.flags |= (1u << _object_has_attached_looping_sounds_bit);
            }
            else if ( attachment_type == _object_attachment_type_effect )
            {
                datum = effect_new_looping(attachment_ref->type.index, object_index,
                    attachment_ref->primary_scale_function_reference - 1,
                    attachment_ref->secondary_scale_function_reference - 1,
                    attachment_ref->change_color_reference - 1);
            }
            else if ( attachment_type == _object_attachment_type_contrail )
            {
                datum = contrail_new(attachment_ref->type.index, object_index, attachment_index);
            }
            else if ( attachment_type != _object_attachment_type_light )  /* == _object_attachment_type_particle_system */
            {
                datum = particle_system_new_attached(attachment_ref->type.index, object_index, attachment_index);
            }
            else                                       /* _object_attachment_type_light */
            {
                datum = light_new(attachment_ref->type.index, object_index, attachment_index,
                    attachment_ref->primary_scale_function_reference - 1,
                    attachment_ref->change_color_reference - 1);
                if ( datum != -1 )
                    object_data->object.flags |= (1u << _object_has_attached_lights_bit);
            }
        }

        attachment_index = (__int16)(slot + 1);
        object_data->object.attachment_types[slot] = attachment_type;
        object_data->object.attachment_indices[slot] = datum;
        slot = attachment_index;
    }
    while ( attachment_index < object_definition->attachments.count );
}
