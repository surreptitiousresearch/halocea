#pragma once
#include "hkString.h"
#include "hkVector4.h"

/* hkdExceptionReporter::DebugText (size 48, DB types_members) — one queued debug
   label: its text, an anchor position and a packed RGBA colour. */
typedef struct hkdExceptionReporter_DebugText
{
    hkString  m_text;       /* 0x00 */
    hkVector4 m_position;   /* 0x10 */
    unsigned int m_color;   /* 0x20 */
} hkdExceptionReporter_DebugText;
