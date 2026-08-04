#pragma once

/* 16-byte cached widget render state (the key a batch is keyed on). */
typedef struct widget_state
{
    int stage_index;        // 0x00
    int bitmap_group_index; // 0x04
    int sequence_index;     // 0x08
    __int16 stencil_mode;   // 0x0C  (padded to 0x10)
} widget_state;
