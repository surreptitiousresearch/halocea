#pragma once
/* _biped_update_header — the message_delta_processor routing header for biped_build_update_delta.c /
 * biped_process_update_delta.c (12 bytes). Sibling shape to _vehicle_update_header
 * (headers/vehicle_update_header.h) plus the pre-clear value of object.force_shield_update.
 * Names and widths are the DB's (types_members biped_update_header): object_index@0,
 * baseline_index@4, message_index@5, update_baseline@6, force_shield_update@7, timestamp@8, all
 * four middle bytes `unsigned __int8`. That agrees with the wire descriptor in
 * src/data/message_header_biped_update_header_definition.c, which serializes +4/+5 through
 * field_properties_integer_small_definition and +6/+7 through field_properties_boolean_definition.
 * Wire-format struct — do not reorder or resize. */

typedef struct _biped_update_header
{
    int           object_index;      /* 0x00 - field_translated_index_translate_index(object_index) */
    unsigned char baseline_index;    /* 0x04 - biped.baseline_index (object +1319) */
    unsigned char message_index;     /* 0x05 - biped.message_index (object +1320), incremented after a successful encode */
    unsigned char update_baseline;   /* 0x06 - set when the encode mode is stateless; gates the decoder's baseline refresh */
    unsigned char force_shield_update; /* 0x07 - pre-clear value of object.force_shield_update (+290) */
    unsigned int  timestamp;         /* 0x08 */
} _biped_update_header;
