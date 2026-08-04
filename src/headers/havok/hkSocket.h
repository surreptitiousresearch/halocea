#pragma once
#include "hkBool.h"

/* hkSocket, size 32 (DB types_members): hkReferencedObject base @0 (8) + two
 * 12-byte stream adapters (m_reader @8, m_writer @20). Each adapter derives from
 * hkStreamReader/hkStreamWriter (themselves just an hkReferencedObject, 8 bytes)
 * and adds a back-pointer m_socket @8. The adapters carry their own vtables that
 * route stream reads/writes through the owning socket. */

typedef struct hkSocket hkSocket; /* forward — adapters point back at the owner */

typedef struct hkSocket_vtbl               hkSocket_vtbl;               /* boundary vtable */
typedef struct hkSocket_ReaderAdapter_vtbl hkSocket_ReaderAdapter_vtbl; /* boundary vtable */
typedef struct hkSocket_WriterAdapter_vtbl hkSocket_WriterAdapter_vtbl; /* boundary vtable */

/* hkSocket::ReaderAdapter (12 bytes) — hkStreamReader base fields inlined
 * (__vftable @0, m_memSizeAndFlags @4, m_referenceCount @6) + m_socket @8. */
typedef struct hkSocket_ReaderAdapter
{
    hkSocket_ReaderAdapter_vtbl *__vftable;  /* 0x00 */
    unsigned short m_memSizeAndFlags;        /* 0x04 */
    short          m_referenceCount;         /* 0x06 */
    hkSocket      *m_socket;                 /* 0x08 */
} hkSocket_ReaderAdapter;

/* hkSocket::WriterAdapter (12 bytes) — mirror of ReaderAdapter over hkStreamWriter. */
typedef struct hkSocket_WriterAdapter
{
    hkSocket_WriterAdapter_vtbl *__vftable;  /* 0x00 */
    unsigned short m_memSizeAndFlags;        /* 0x04 */
    short          m_referenceCount;         /* 0x06 */
    hkSocket      *m_socket;                 /* 0x08 */
} hkSocket_WriterAdapter;

struct hkSocket
{
    hkSocket_vtbl *__vftable;             /* 0x00 — hkReferencedObject base */
    unsigned short m_memSizeAndFlags;     /* 0x04 */
    short          m_referenceCount;      /* 0x06 */
    hkSocket_ReaderAdapter m_reader;      /* 0x08 */
    hkSocket_WriterAdapter m_writer;      /* 0x14 */
};

/* One-time platform network stack init. DB exposes these static NAMES only. */
extern hkBool s_platformNetInitialized;      /* hkSocket::s_platformNetInitialized */
extern void (*s_platformNetInit)(void);      /* hkSocket::s_platformNetInit */
extern void *const hkSocket_vftable;               /* hkSocket::`vftable' */
extern void *const hkSocket_ReaderAdapter_vftable; /* hkSocket::ReaderAdapter::`vftable' */
extern void *const hkSocket_WriterAdapter_vftable; /* hkSocket::WriterAdapter::`vftable' */

void hkSocket_construct(hkSocket *self); /* hkSocket::hkSocket */
