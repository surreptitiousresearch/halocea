#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* ai_communication_candidate — one weighted speech/reply candidate built on the stack by
 * ai_communication_event (56 bytes, 16-entry local array). No DB type exists (it is a purely
 * function-local record); the layout is defined by the function's own construction code and is
 * reproduced field-for-field. */

typedef struct ai_communication_candidate
{
    float   weight;                   /* 0x00 */
    unsigned __int8 interrupts;       /* 0x04 — row flag bit 1 */
    unsigned __int8 is_reply;         /* 0x05 */
    __int16 vocalization_type;        /* 0x06 */
    __int16 priority;                 /* 0x08 */
    __int16 animation_type;           /* 0x0A */
    __int16 considered_vocalization;  /* 0x0C */
    __int16 delay_time;               /* 0x0E */
    __int16 ai_delay_time;            /* 0x10 */
    unsigned char _pad12[2];          /* 0x12 */
    int     protagonist_unit_index;   /* 0x14 */
    int     protagonist_actor_index;  /* 0x18 */
    int     recipient_unit_index;     /* 0x1C */
    int     reply_actor_index;        /* 0x20 */
    __int16 protagonist_look_priority;/* 0x24 */
    __int16 recipient_look_priority;  /* 0x26 */
    __int16 look_target_type;         /* 0x28 */
    unsigned char _pad2A[2];          /* 0x2A */
    int     look_unit_index;          /* 0x2C */
    int     sound_definition_index;   /* 0x30 */
    __int16 table_row_index;          /* 0x34 */
    unsigned char _pad36[2];          /* 0x36 */
} ai_communication_candidate;         /* 56 bytes */
