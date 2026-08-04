/* hs_token_enumerators @ 0x841787A8 — .data data-init: 18 haloscript token-enumerator
 * callbacks (void (*)(void)), one per hs_token_type (see headers/hs_token_type.h),
 * indexed by hs_tokens_enumerate via its type_flags bitmask. */
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
