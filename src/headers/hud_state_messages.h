#pragma once
/* hud_state_messages — HUD state messages tag ('hmt '); DB-verified layout (128 bytes). */

#include "tag_data.h"
#include "tag_block.h"

typedef struct hud_state_messages
{
    tag_data  text_data;   /* 0x00 */
    tag_block elements;    /* 0x14 — per-element records */
    tag_block messages;    /* 0x20 — hud_state_message_definition[] (64-byte records) */
    int       unused[21];  /* 0x2C */
} hud_state_messages;      /* 128 bytes */
