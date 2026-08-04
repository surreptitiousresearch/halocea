#pragma once

#include "hkBool.h"
#include "hkPadSpu.h"

/* hkMonitorStream — per-thread profiling capture buffer. Timer/value commands
   are appended at m_end; a command is only written when there is still room
   (m_end < m_capacityMinus16). Layout verified against the DB
   (types_members: hkMonitorStream, ::Command, ::TimerCommand). */

/* types_members hkMonitorStream::Command */
typedef struct hkMonitorStream_Command
{
    const char *m_commandAndMonitor;
} hkMonitorStream_Command;

/* Local alias so the base spelling matches the DB's nested-qualified type
   (hkMonitorStream::Command). */
typedef hkMonitorStream_Command Command;

/* types_members hkMonitorStream::TimerCommand (12 bytes) — Command is a BASE
   CLASS in the DB, not a named member. */
struct hkMonitorStream_TimerCommand : Command
{
    unsigned int m_time0;   /* 0x4 */
    unsigned int m_time1;   /* 0x8 */
};
typedef struct hkMonitorStream_TimerCommand hkMonitorStream_TimerCommand;

typedef struct hkMonitorStream
{
    hkPadSpu<char *> m_start;             /* +0  */
    hkPadSpu<char *> m_end;               /* +4  current write cursor */
    hkPadSpu<char *> m_capacity;          /* +8  */
    hkPadSpu<char *> m_capacityMinus16;   /* +12 write guard limit */
    hkBool m_isBufferAllocatedOnTheHeap;  /* +16 */
} hkMonitorStream;
