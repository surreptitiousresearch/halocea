#pragma once
/* actor_destination_orders — member of actor_move_orders and actor_path_control_data, 24 bytes.
 * Reconciled to DB-verified layout. The destination payload is the anonymous union
 * _89B12494B82246BF2F30085E70EEA39E, named ___u3 in the DB, discriminated by destination_type.
 * $-names kept verbatim ($ in identifiers is an MSVC extension), matching the mtlPARAM.h convention. */

#include <stdint.h>
#include "real_point3d.h"

/* DB anonymous struct arm of $89B12494... — raw point destination */
typedef struct _39977C2A29D512E6723718F83A4242A3
{
    real_point3d point;           /* 0x00 */
    int surface_index;            /* 0x0C */
} _39977C2A29D512E6723718F83A4242A3;

/* DB anonymous struct arm of $89B12494... — prop destination */
typedef struct _16614D93AB4173806BD44D148EA915E7
{
    int prop_index;               /* 0x00 */
    float accept_radius;          /* 0x04 */
} _16614D93AB4173806BD44D148EA915E7;

/* DB anonymous union _89B12494B82246BF2F30085E70EEA39E (16 bytes) */
typedef union _89B12494B82246BF2F30085E70EEA39E
{
    struct _39977C2A29D512E6723718F83A4242A3 raw;   /* 0x00 */
    int16_t firing_position_index;                  /* 0x00 */
    int16_t move_position_index;                    /* 0x00 */
    struct _16614D93AB4173806BD44D148EA915E7 prop;  /* 0x00 */
} _89B12494B82246BF2F30085E70EEA39E;

typedef struct actor_destination_orders
{
    int16_t destination_type;         /* 0x00 — discriminant: what type of destination is specified */
    uint8_t keep_moving;      /* 0x02 */
    uint8_t pad;              /* 0x03 */
    _89B12494B82246BF2F30085E70EEA39E ___u3; /* 0x04 — destination payload union */
    int ignore_target_object_index;   /* 0x14 */
} actor_destination_orders; /* 24 bytes */
