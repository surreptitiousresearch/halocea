/* hs_type_sizes @0x82118F10 (.rdata) - value size in bytes for every HaloScript node/value type,
 * indexed by hs_type (0..48). 49 int16_t entries = 98 bytes; +0x62..+0x63 are zero alignment
 * padding inside the 100-byte slot (_hs_type_real_default follows at +0x64). Read as
 * `slwi idx,1` + `lhzx` + `extsh` in hs_evaluate_equality @0x8368F8F0 -> 2-byte signed elements,
 * matching the corpus decl `const int16_t hs_type_sizes[]`. */
#include <stdint.h>
#include "../headers/hs_type.h"

const int16_t hs_type_sizes[number_of_hs_node_types] =
{
    0, 0, 0, 0,            /* [0..3]   hs_unparsed, hs_special_form, hs_function_name, hs_passthrough */
    0,                     /* [4]      hs_type_void */
    1,                     /* [5]      hs_type_boolean */
    4,                     /* [6]      hs_type_real */
    2,                     /* [7]      hs_type_short_integer */
    4,                     /* [8]      hs_type_long_integer */
    4,                     /* [9]      hs_type_string */
    4,                     /* [10]     hs_type_script */
    2, 2, 2, 2, 2, 2,      /* [11..16] trigger_volume, cutscene_flag, cutscene_camera_point,
                                       cutscene_title, cutscene_recording, device_group */
    4,                     /* [17]     hs_type_ai */
    2, 2, 2, 2, 2,         /* [18..22] ai_command_list, starting_profile, conversation, navpoint,
                                       hud_message */
    4,                     /* [23]     hs_type_object_list */
    4, 4, 4, 4, 4, 4, 4, 4,/* [24..31] first_hs_tag_reference_type..last_hs_tag_reference_type */
    2, 2, 2, 2, 2,         /* [32..36] first_hs_enum_type..last_hs_enum_type */
    4, 4, 4, 4, 4, 4,      /* [37..42] first_hs_object_type..last_hs_object_type */
    2, 2, 2, 2, 2, 2       /* [43..48] first_hs_object_name_type..last_hs_object_name_type */
};
