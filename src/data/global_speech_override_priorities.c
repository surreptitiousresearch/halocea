/* global_speech_override_priorities @ 0x821263C4 - .rdata read-only table, 24 bytes.
 * unit_speech_priority -> unit_speech_priority: the priority a request must out-rank before it may
 * pre-empt whatever the unit is already saying (unit_test_speech.c:77 - `override_priority <
 * effective_priority`). Every entry is <= its own index, so it is a demotion ladder.
 *
 * DEVIATION: the sole decl, `extern int global_speech_override_priorities[]`
 * (src/unit_test_speech.c:24), is WRONG - the table is 16-bit. The disassembly indexes it with a
 * 1-bit shift and a halfword load:
 *   837BA814  lis   r10, global_speech_override_priorities@ha
 *   837BA818  slwi  r31, r9, 1
 *   837BA824  lhzx  r11, r31, r10
 *   837BA828  extsh r11, r11            (so: signed 16-bit)
 * and the raw big-endian dwords are incoherent as int32 (0x00010001, 0x00050005,
 * 0x000A0000) but a clean ladder as int16. An `int` decl strides 4x and fuses two entries per read. Defined here as
 * int16_t; the decl is normalized corpus-wide by another pass.
 *
 * 11 entries == NUMBER_OF_UNIT_SPEECH_PRIORITIES == 22 bytes; the last halfword (+0x16 = 0x0000)
 * is alignment padding ahead of global_speech_queue_times @0x821263DC and is not an element.
 *
 * raw -> resolved:
 *   +0x00 0x0000 ->  0 _unit_speech_none   [_unit_speech_none]
 *   +0x02 0x0000 ->  0 _unit_speech_none   [_unit_speech_idle]
 *   +0x04 0x0001 ->  1 _unit_speech_idle   [_unit_speech_pain]
 *   +0x06 0x0001 ->  1 _unit_speech_idle   [_unit_speech_talk]
 *   +0x08 0x0002 ->  2 _unit_speech_pain   [_unit_speech_communicate]
 *   +0x0A 0x0002 ->  2 _unit_speech_pain   [_unit_speech_shout]
 *   +0x0C 0x0005 ->  5 _unit_speech_shout   [_unit_speech_scripted]
 *   +0x0E 0x0005 ->  5 _unit_speech_shout   [_unit_speech_involuntary]
 *   +0x10 0x0007 ->  7 _unit_speech_involuntary   [_unit_speech_exclamation]
 *   +0x12 0x0007 ->  7 _unit_speech_involuntary   [_unit_speech_scream]
 *   +0x14 0x000A -> 10 _unit_speech_death   [_unit_speech_death]
 */
#include <stdint.h>
#include "../headers/unit_speech_priority.h"

int16_t global_speech_override_priorities[NUMBER_OF_UNIT_SPEECH_PRIORITIES] =
{
    _unit_speech_none,         /* [_unit_speech_none] */
    _unit_speech_none,         /* [_unit_speech_idle] */
    _unit_speech_idle,         /* [_unit_speech_pain] */
    _unit_speech_idle,         /* [_unit_speech_talk] */
    _unit_speech_pain,         /* [_unit_speech_communicate] */
    _unit_speech_pain,         /* [_unit_speech_shout] */
    _unit_speech_shout,        /* [_unit_speech_scripted] */
    _unit_speech_shout,        /* [_unit_speech_involuntary] */
    _unit_speech_involuntary,  /* [_unit_speech_exclamation] */
    _unit_speech_involuntary,  /* [_unit_speech_scream] */
    _unit_speech_death,        /* [_unit_speech_death] */
};
