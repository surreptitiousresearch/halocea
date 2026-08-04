#pragma once
#include "hkReferencedObject.h"
#include "hkArray.h"
#include "hkSingleton.h"

typedef struct hkOstream hkOstream;

/* types_members hkTraceStream::Title (32 bytes) */
typedef struct hkTraceStream_Title
{
    char m_value[32]; /* 0x00 */
} hkTraceStream_Title;

/* Local alias so member spellings match the DB's nested-qualified template
   argument (hkArray<hkTraceStream::Title>). */
typedef hkTraceStream_Title Title;

/* size 28 — hkReferencedObject base + empty hkSingleton<hkTraceStream> base. */
struct hkTraceStream : hkReferencedObject, hkSingleton<hkTraceStream>
{
    hkOstream *m_stream;        /* offset 8  */
    int m_counter;              /* offset 12 */
    hkArray<Title> m_titles;    /* offset 16 */
};
typedef struct hkTraceStream hkTraceStream;
