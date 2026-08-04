#pragma once
/* animation_event_header — 1-byte bitfield tag at the start of an animation event stream entry. */
typedef struct animation_event_header {
    unsigned char event_time_type : 2; /* bits [0:1] */
    unsigned char event_type      : 6; /* bits [2:7] */
} animation_event_header;
