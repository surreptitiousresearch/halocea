/* hs_token_enumerators @ 0x841787A8 (.data, 72 bytes)
 * DB applied_types: void (__fastcall *hs_token_enumerators[18])();
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8372F388 -> hs_enumerate_special_form_names
 *   +0x0004 = 0x8372F3C0 -> hs_enumerate_script_type_names
 *   +0x0008 = 0x8372F3D8 -> hs_enumerate_type_names
 *   +0x000C = 0x8372F3F0 -> hs_enumerate_function_names
 *   +0x0010 = 0x8372F4A8 -> hs_enumerate_script_names
 *   +0x0014 = 0x8372F4D8 -> hs_enumerate_variable_names
 *   +0x0018 = 0x8372F568 -> hs_enumerate_ai_names
 *   +0x001C = 0x8372F598 -> hs_enumerate_ai_command_list_names
 *   +0x0020 = 0x8372F5C8 -> hs_enumerate_starting_profile_names
 *   +0x0024 = 0x8372F5F8 -> hs_enumerate_conversation_names
 *   +0x0028 = 0x8372F628 -> hs_enumerate_object_names
 *   +0x002C = 0x8372F658 -> hs_enumerate_trigger_volume_names
 *   +0x0030 = 0x8372F688 -> hs_enumerate_cutscene_flag_names
 *   +0x0034 = 0x8372F6B8 -> hs_enumerate_cutscene_camera_point_names
 *   +0x0038 = 0x8372F6E8 -> hs_enumerate_cutscene_title_names
 *   +0x003C = 0x8372F718 -> hs_enumerate_cutscene_recording_names
 *   +0x0040 = 0x8372F748 -> hs_enumerate_navpoints
 *   +0x0044 = 0x8372F7A0 -> hs_enumerate_hud_messages
 * .data data-init: 18 haloscript token-enumerator
 * callbacks (void (*)(void)), one per hs_token_type (see headers/hs_token_type.h),
 * indexed by hs_tokens_enumerate via its type_flags bitmask.
 */
extern void hs_enumerate_special_form_names(void);          /* 0x8372F388 */
extern void hs_enumerate_script_type_names(void);           /* 0x8372F3C0 */
extern void hs_enumerate_type_names(void);                  /* 0x8372F3D8 */
extern void hs_enumerate_function_names(void);              /* 0x8372F3F0 */
extern void hs_enumerate_script_names(void);                /* 0x8372F4A8 */
extern void hs_enumerate_variable_names(void);              /* 0x8372F4D8 */
extern void hs_enumerate_ai_names(void);                    /* 0x8372F568 */
extern void hs_enumerate_ai_command_list_names(void);       /* 0x8372F598 */
extern void hs_enumerate_starting_profile_names(void);      /* 0x8372F5C8 */
extern void hs_enumerate_conversation_names(void);          /* 0x8372F5F8 */
extern void hs_enumerate_object_names(void);                /* 0x8372F628 */
extern void hs_enumerate_trigger_volume_names(void);        /* 0x8372F658 */
extern void hs_enumerate_cutscene_flag_names(void);         /* 0x8372F688 */
extern void hs_enumerate_cutscene_camera_point_names(void); /* 0x8372F6B8 */
extern void hs_enumerate_cutscene_title_names(void);        /* 0x8372F6E8 */
extern void hs_enumerate_cutscene_recording_names(void);    /* 0x8372F718 */
extern void hs_enumerate_navpoints(void);                   /* 0x8372F748 */
extern void hs_enumerate_hud_messages(void);                /* 0x8372F7A0 */

void (*hs_token_enumerators[18])(void) = {
    hs_enumerate_special_form_names,          /* _hs_token_type_form */
    hs_enumerate_script_type_names,           /* _hs_token_type_script_type */
    hs_enumerate_type_names,                  /* _hs_token_type_type */
    hs_enumerate_function_names,              /* _hs_token_type_function */
    hs_enumerate_script_names,                /* _hs_token_type_script */
    hs_enumerate_variable_names,              /* _hs_token_type_variable */
    hs_enumerate_ai_names,                    /* _hs_token_type_ai */
    hs_enumerate_ai_command_list_names,       /* _hs_token_type_ai_command_list */
    hs_enumerate_starting_profile_names,      /* _hs_token_type_starting_profile */
    hs_enumerate_conversation_names,          /* _hs_token_type_conversation */
    hs_enumerate_object_names,                /* _hs_token_type_object */
    hs_enumerate_trigger_volume_names,        /* _hs_token_type_trigger_volume */
    hs_enumerate_cutscene_flag_names,         /* _hs_token_type_cutscene_flag */
    hs_enumerate_cutscene_camera_point_names, /* _hs_token_type_cutscene_camera_point */
    hs_enumerate_cutscene_title_names,        /* _hs_token_type_cutscene_title */
    hs_enumerate_cutscene_recording_names,    /* _hs_token_type_cutscene_recording */
    hs_enumerate_navpoints,                   /* _hs_token_type_navpoint */
    hs_enumerate_hud_messages,                /* _hs_token_type_hud_message */
};
