#ifndef HS_TOKEN_TYPE_H
#define HS_TOKEN_TYPE_H

/* hs_token_type — the token kinds enumerated by hs_tokens_enumerate (one enumerator callback per
 * kind in hs_token_enumerators[]). Selected by the type_flags bitmask (1 << _hs_token_type_*).
 * Source: DB anonymous enum _F4BD23825D73E80916C07B0CE86A4AA1 (types_enum_values, verbatim). */
enum hs_token_type
{
    _hs_token_type_form                  = 0,
    _hs_token_type_script_type           = 1,
    _hs_token_type_type                  = 2,
    _hs_token_type_function              = 3,
    _hs_token_type_script                = 4,
    _hs_token_type_variable              = 5,
    _hs_token_type_ai                    = 6,
    _hs_token_type_ai_command_list       = 7,
    _hs_token_type_starting_profile      = 8,
    _hs_token_type_conversation          = 9,
    _hs_token_type_object                = 10,
    _hs_token_type_trigger_volume        = 11,
    _hs_token_type_cutscene_flag         = 12,
    _hs_token_type_cutscene_camera_point = 13,
    _hs_token_type_cutscene_title        = 14,
    _hs_token_type_cutscene_recording    = 15,
    _hs_token_type_navpoint              = 16,
    _hs_token_type_hud_message           = 17,
    NUMBER_OF_HS_TOKEN_TYPES             = 18
};

#endif /* HS_TOKEN_TYPE_H */
