#pragma once
/* Per-slot attachment type byte. Ground-truth DB enum
 * (types_enum_values _B6DEA37A83789BF785DFE74D7A1A4184, cited below). The dispatch in
 * attachments_new / attachments_delete matches it exactly:
 *   0 -> light, 1 -> looping_sound, 2 -> effect, 3 -> contrail, 4 -> particle_system.
 * 255 is the sentinel for an empty slot (not part of the DB enum). */

/* layout bound to DB enum: types_enum_values _B6DEA37A83789BF785DFE74D7A1A4184 */
enum object_attachment_type
{
	_object_attachment_type_light = 0x0,
	_object_attachment_type_looping_sound = 0x1,
	_object_attachment_type_effect = 0x2,
	_object_attachment_type_contrail = 0x3,
	_object_attachment_type_particle_system = 0x4,
	NUMBER_OF_OBJECT_ATTACHMENT_TYPES = 0x5,
};

/* not part of the DB enum: the 0xFF "no attachment" sentinel observed in attachments_delete */
#define _object_attachment_type_none 0xFF

