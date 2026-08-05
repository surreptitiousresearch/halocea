#pragma once
/* event_record — a single UI input event (input.c). DB layout: the 4-byte payload at +4 is the
 * anonymous union event_record::<unnamed_type_value> with a point2d axis view and a nested
 * _button_value view. The DB anonymous-type names (event_record::<unnamed_type_value>,
 * ...::_button_value) are IDA/MSVC dump notation and not legal C identifiers; they are renamed
 * below to event_record_value / event_record_button_value with the layout kept byte-identical
 * (types_members event_record::<unnamed_type_value>). */

#include <stdint.h>
#include "point2d.h"

/* DB struct event_record::<unnamed_type_value>::_button_value — button id + pressed flag (2 bytes). */
typedef struct event_record_button_value
{
    uint8_t button; /* 0x0 */
    uint8_t value;  /* 0x1 */
} event_record_button_value;

/* DB union event_record::<unnamed_type_value> — the event payload overlay (4 bytes). */
typedef union event_record_value
{
    point2d analog_value;                    /* 0x0 — n[0],n[1] axis values */
    event_record_button_value button_value;  /* 0x0 */
    int     value; /* recovered source alias (whole-payload int view); not a DB member */
} event_record_value;

typedef struct event_record
{
    int16_t type;             /* 0x0 — 1=axis0, 2=axis1, 3=button, 4=char */
    int16_t controller_index; /* 0x2 */
    event_record_value value; /* 0x4 */
} event_record;               /* 8 bytes */
