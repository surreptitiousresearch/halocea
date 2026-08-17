#pragma once
#include "hkString.h"
#include "hkObjectArray.h"

/* hkdExceptionReporter::ErrorMessage (size 104, DB types_members) — one captured
   error, carrying its description plus six debug-primitive arrays that the sink
   accumulates for later replay. The Debug* element structs are only needed here
   for their byte sizes (array deallocation) and, for DebugText, its element
   destructor, so they are modelled as fixed-size opaque blobs. */
typedef struct hkdExceptionReporter_DebugPoint    { unsigned char _b[32]; } hkdExceptionReporter_DebugPoint;    /* 32 */
typedef struct hkdExceptionReporter_DebugLine     { unsigned char _b[48]; } hkdExceptionReporter_DebugLine;     /* 48 */
typedef struct hkdExceptionReporter_DebugTriangle { unsigned char _b[64]; } hkdExceptionReporter_DebugTriangle; /* 64 */
typedef struct hkdExceptionReporter_DebugPlane    { unsigned char _b[48]; } hkdExceptionReporter_DebugPlane;    /* 48 */
#include "hkdExceptionReporter_DebugText.h"  /* typed 48-byte layout; was an opaque blob restated here (ODR dup) */
typedef struct hkdExceptionReporter_DebugFrame    { unsigned char _b[80]; } hkdExceptionReporter_DebugFrame;    /* 80 */

typedef struct hkdExceptionReporter_ErrorMessage
{
    int m_type;                                                        /* 0   hkError::Message */
    int m_id;                                                          /* 4   */
    hkString m_description;                                            /* 8   */
    const char *m_file;                                               /* 20  */
    int m_line;                                                       /* 24  */
    int m_debugType;                                                  /* 28  hkdExceptionReporter::DebugMessageType */
    hkObjectArray<hkdExceptionReporter_DebugPoint>    m_points;       /* 32  */
    hkObjectArray<hkdExceptionReporter_DebugLine>     m_lines;        /* 44  */
    hkObjectArray<hkdExceptionReporter_DebugTriangle> m_triangles;    /* 56  */
    hkObjectArray<hkdExceptionReporter_DebugPlane>    m_planes;       /* 68  */
    hkObjectArray<hkdExceptionReporter_DebugText>     m_texts;        /* 80  */
    hkObjectArray<hkdExceptionReporter_DebugFrame>    m_frames;       /* 92  */
} hkdExceptionReporter_ErrorMessage;                                  /* 104 */

void hkdExceptionReporter_ErrorMessage_destruct(hkdExceptionReporter_ErrorMessage *self); /* hkdExceptionReporter::ErrorMessage::~ErrorMessage */
