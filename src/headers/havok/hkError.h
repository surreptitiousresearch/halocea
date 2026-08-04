#pragma once
#include "hkReferencedObject.h"
#include "hkOstream.h"
#include "hkSingleton.h"

/* hkErrStream : hkOstream — streams formatted text into a caller buffer, then
   hands it to the global hkError sink. */
typedef struct hkErrStream { hkOstream base; } hkErrStream;

/* size 8 — global error sink. DB bases: hkReferencedObject @0 (8) + hkSingleton<hkError> @8
   (zero-size empty base; spelled as a member here, does not contribute bytes in the binary). */
typedef struct hkError
{
    hkReferencedObject   base;           /* 0x00 */
    hkSingleton<hkError> singleton_base; /* 0x08 — zero-size base class in DB */
} hkError;

/* DB exposes these enumerator/singleton NAMES, not their values/addresses. */
extern const int MESSAGE_ERROR;
extern const int MESSAGE_WARNING;
extern const int MESSAGE_REPORT;
extern hkError *hkError_s_instance; /* hkSingleton<hkError>::s_instance */

void hkErrStream_construct(hkErrStream *self, char *buffer, int bufferSize); /* hkErrStream::hkErrStream */
void hkOstream_destruct(hkOstream *self);                                    /* hkOstream::~hkOstream */
hkOstream *hkOstream_insertString(hkOstream *self, const char *text);        /* operator<<(const char*) */
hkOstream *hkOstream_insertObject(hkOstream *self, const hkReferencedObject *object); /* operator<<(const hkReferencedObject*) */
hkOstream *hkOstream_insertInt(hkOstream *self, int value);                  /* operator<<(int) */
int hkError_message(hkError *self, int type, int id, const char *text, const char *file, int line); /* virtual hkError::message */
void __trap(void);                                                           /* PPC trap intrinsic */
