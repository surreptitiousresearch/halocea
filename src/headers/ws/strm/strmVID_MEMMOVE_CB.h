#pragma once
// ws-engine strm: async-compaction move-notify callback interface a vid-memory buffer owner
// implements so the defragmenter can tell it its backing storage moved.
// DB-verified layout (types_members strmVID_MEMMOVE_CB): __vftable@0 — size 4.
// DB types_members strmVID_MEMMOVE_CB_vtbl: dtor@0, OnMemmoveNotify@4.

struct strmVID_BUFFER;
struct strmVID_MEMMOVE_CB;

typedef struct strmVID_MEMMOVE_CB_vtbl {
    void (*dtr_strmVID_MEMMOVE_CB)(strmVID_MEMMOVE_CB *self);              // 0x00
    void (*OnMemmoveNotify)(strmVID_MEMMOVE_CB *self, strmVID_BUFFER *);   // 0x04
} strmVID_MEMMOVE_CB_vtbl;

typedef struct strmVID_MEMMOVE_CB {
    strmVID_MEMMOVE_CB_vtbl *__vftable; // 0x00
} strmVID_MEMMOVE_CB;
