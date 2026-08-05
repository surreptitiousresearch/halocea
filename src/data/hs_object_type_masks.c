/* hs_object_type_masks @0x82118EE0 (.rdata) - 12 bytes = six int16_t, one per HaloScript object
 * script type: a bitmask of the object_type values that script type accepts. Indexed by
 * (type - first_hs_object_type) for the object bank and (type - first_hs_object_name_type) for the
 * object-name bank, so the same six masks serve both. Exactly six entries, no padding
 * (hs_tag_reference_type_group_tags follows at +0x0C). Every value matches a named object_mask_*
 * constant of enum object_type.
 * DEVIATION: read as `slwi idx,1` + `lhzx` + `extsh` (hs_object_type_can_cast @0x8368D6D8,
 * hs_can_cast @0x8368D718) -> 2-byte signed elements, while src/headers/hs_object_type_masks.h
 * declares `const unsigned int []`, which is 4x too wide (only three words exist at that width for
 * six script types). Defined at the binary's width. */
#include <stdint.h>
#include "../headers/hs_type.h"
#include "../headers/object_type.h"

const int16_t hs_object_type_masks[number_of_hs_object_types] =
{
    object_mask_all,        /* 0xFFFF  hs_type_object  (+ hs_type_object_name) */
    object_mask_unit,       /* 0x0003  hs_type_unit    (biped | vehicle) */
    object_mask_vehicle,    /* 0x0002  hs_type_vehicle */
    object_mask_weapon,     /* 0x0004  hs_type_weapon */
    object_mask_device,     /* 0x0380  hs_type_device  (machine | control | light_fixture) */
    object_mask_scenery     /* 0x0040  hs_type_scenery */
};
