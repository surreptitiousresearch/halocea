#pragma once
#include "hkReferencedObject.h"
typedef struct hkStreamWriter hkStreamWriter;
/* size 12 */
typedef struct hkOstream
{
    hkReferencedObject base; /* offset 0 */
    hkStreamWriter *m_writer;/* offset 8 */
} hkOstream;
