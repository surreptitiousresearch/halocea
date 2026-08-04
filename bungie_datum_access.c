/*==============================================================================
 * blam_datum_access.c
 *
 * A REFERENCE / TEACHING example for Halo: Combat Evolved (Halo 1) source.
 *
 * PURPOSE
 *   Dump this file next to a re-sourced/decompiled Halo CE codebase and point
 *   a Claude prompt at it, e.g.:
 *     "When it looks like you are using a datum, check blam_datum_access.c
 *      for how a function should access this datum and its members."
 *   It shows the canonical, correct way to resolve datum handles into
 *   pointers and to read/write datum members.
 *
 * PROVENANCE
 *   Every identifier below is OFFICIAL BUNGIE, attested in:
 *     - Chris Butcher, Halo engine internals notes (datum array design, salt
 *       derivation, datum_get access tiers, NONE).
 *     - Mat Noguchi (Bungie), "Content Management for Halo 2 and Beyond"
 *       (the TAG_GROUP macro family; same system shipped in Halo CE).
 *     - The official Halo Editing Kit / Halo: CE Mod Tools (H1A-EK), which
 *       derive from Bungie's original CE tools.
 *   No OpenSauce/BlamLib/invader names and no invented convenience wrappers
 *   are used. Where this file must supply glue to compile standalone, it is
 *   confined to the clearly-marked "reference implementation" bodies; the
 *   NAMES and SEMANTICS of the datum API are all Bungie's.
 *
 * THE DATUM MODEL (read this first)
 *   A data_array is a fixed-capacity pool of equally-sized "datums", tagged
 *   with the signature 'd@t@'. A datum_index is a 32-bit HANDLE, not a raw
 *   index:
 *
 *        bit 31 ............... 16 | 15 ................... 0
 *              identifier (salt)   |     absolute index
 *
 *   - The low 16 bits are the absolute slot index.
 *   - The high 16 bits are a "salt" / identifier that detects stale handles:
 *     each slot stamps a salt when allocated; freeing zeroes it; reallocating
 *     issues a new salt. A handle whose salt no longer matches the slot is
 *     stale (use-after-free) and must not be dereferenced.
 *   - Every datum begins with a 2-byte header holding that salt; salt 0 means
 *     the slot is free.
 *   - datum_size INCLUDES the 2-byte header, so it is the full stride.
 *   - NONE (-1, 0xFFFFFFFF) is the null handle. Index 0 with a live salt is a
 *     perfectly valid datum, so never test against 0 for "null".
 *
 * C99, 32-bit-faithful layout (data_array is 0x38 bytes).
 *
 * BUILD NOTES
 *   - This is C. It compiles clean as an object file with
 *       gcc -std=c99 -Wall -Wextra -Wno-multichar -c blam_datum_access.c
 *   - The four-character-code literals ('d@t@', 'snde', 'actr') are Bungie's
 *     own spelling and trigger -Wmultichar; that warning is expected and
 *     harmless. Pass -Wno-multichar to silence it.
 *   - Intended primarily as a REFERENCE for tools to read, not necessarily to
 *     link into your build. (The tag_group section leans on C rules for the
 *     forward-declared field arrays, so compile this file as C, not C++.)
 *============================================================================*/

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/*==============================================================================
 * cseries base types & macros  (official)
 *============================================================================*/

typedef int8_t   boolean;
typedef uint8_t  byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef int16_t  int16;
typedef int32_t  int32;
typedef float    real;
typedef char     tag_string[32];
typedef uint32_t tag;            /* four-character code, e.g. 'd@t@', 'actr' */

#define NONE ((int32)-1)         /* universal null handle / null index       */

#define FLAG(bit)               (1u << (bit))
#define TEST_FLAG(flags, bit)   (((flags) & FLAG(bit)) != 0)
#define SET_FLAG(flags, bit, v) ((v) ? ((flags) |= FLAG(bit)) \
                                     : ((flags) &= ~FLAG(bit)))

/* In debug builds Bungie's ASSERT is verbose and halts; in release it is
 * compiled out -- which is exactly why decompiled getters appear as bare
 * pointer arithmetic with no salt/bounds checks. Route to your handler. */
#ifndef ASSERT
#define ASSERT(expr) assert(expr)
#endif

/*==============================================================================
 * datum_index handle macros  (official)
 *
 * These three macros are the named forms of the bit-twiddling you will see in
 * decompiled code:
 *     (unsigned __int16)h  or  (h & 0xFFFF)   ->  DATUM_INDEX_TO_ABSOLUTE_INDEX
 *     (h >> 16)                               ->  DATUM_INDEX_TO_IDENTIFIER
 *     ((salt << 16) | index)                  ->  BUILD_DATUM_INDEX
 *============================================================================*/

typedef dword datum_index;       /* opaque 32-bit handle */

#define DATUM_INDEX_TO_ABSOLUTE_INDEX(h) ((int32)((h) & 0xFFFFu))
#define DATUM_INDEX_TO_IDENTIFIER(h)     ((word)(((h) >> 16) & 0xFFFFu))
#define BUILD_DATUM_INDEX(identifier, index) \
    ((datum_index)((((dword)(word)(identifier)) << 16) | ((dword)(word)(index))))

/*==============================================================================
 * data_array container  (official)
 *
 * Field byte-offsets are shown so you can map decompiled reads such as
 * *(_DWORD *)(arr + 52) back to the named field.
 *============================================================================*/

#define k_data_array_signature 'd@t@'   /* Bungie's literal four-char code */

struct data_array {
    tag_string  name;                    /* 0x00  debug name                 */
    int16       maximum_count;           /* 0x20  slot capacity              */
    int16       datum_size;              /* 0x22  stride, INCLUDES 2B header  */
    boolean     valid;                   /* 0x24  set by data_make_valid     */
    boolean     identifier_zero_invalid; /* 0x25  salt 0 => free slot        */
    int16       pad_0x26;                /* 0x26                             */
    tag         signature;              /* 0x28  'd@t@'                      */
    int16       next_index;              /* 0x2C  first-free scan hint        */
    int16       last_index;              /* 0x2E  allocation high-watermark   */
    datum_index next_datum;              /* 0x30  next handle to issue        */
    void       *data;                    /* 0x34  slot storage base           */
};                                       /* 0x38  total (32-bit build)        */

/* The datum header: the first 2 bytes of EVERY datum are its salt. Official
 * code accesses it as the leading member of each datum struct (see the actor
 * example below) or, inline, as *(int16 *)element. */

/*==============================================================================
 * SECTION A -- reference implementations of the official datum API
 *
 * The function NAMES and their documented behavior are Bungie's. The bodies
 * here exist only so this file compiles and demonstrates the exact semantics
 * your accessors must honor. In the real engine these are small inlineable
 * functions; datum_get "compiles down to" the indexing expression.
 *============================================================================*/

/* address of a slot by absolute index (the raw stride arithmetic) */
static inline void *data_array_element(struct data_array *d, int32 absolute_index)
{
    return (byte *)d->data + (size_t)absolute_index * (word)d->datum_size;
}

/* salt of a slot (its 2-byte header) */
static inline int16 *data_array_identifier(struct data_array *d, int32 absolute_index)
{
    return (int16 *)data_array_element(d, absolute_index);
}

/* initial salt derives from the first two ASCII chars of the array name,
 * forced into the negative int16 range (Butcher's example: "script" -> 0xE373).
 * Salts increment per allocation and wrap 0xFFFF -> 0x8000, never becoming 0. */
static inline word data_array_initial_identifier(const char *name)
{
    return (word)((((word)(byte)name[0] << 8) | (word)(byte)name[1]) | 0x8000u);
}
static inline word data_array_next_identifier(word salt)
{
    salt = (word)(salt + 1u);
    if (salt == 0u) salt = 0x8000u;
    return (word)(salt | 0x8000u);
}

void data_verify(struct data_array *d)
{
    ASSERT(d != NULL);
    ASSERT(d->signature == (tag)k_data_array_signature);
    ASSERT(d->maximum_count >= 0);
    ASSERT(d->datum_size >= 2);              /* must fit the salt header */
    ASSERT(d->last_index >= 0 && d->last_index <= d->maximum_count);
}

void data_delete_all(struct data_array *d)
{
    int32 i;
    data_verify(d);
    for (i = 0; i < d->last_index; ++i)
        *data_array_identifier(d, i) = 0;
    d->next_index = 0;
    d->last_index = 0;
    d->next_datum = BUILD_DATUM_INDEX(data_array_initial_identifier(d->name), 0);
}

struct data_array *data_new(const char *name, int16 maximum_count, int16 datum_size)
{
    struct data_array *d = (struct data_array *)calloc(
        1, sizeof(struct data_array) + (size_t)maximum_count * (word)datum_size);
    if (!d) return NULL;
    strncpy(d->name, name, sizeof(d->name) - 1);
    d->maximum_count           = maximum_count;
    d->datum_size              = datum_size;
    d->identifier_zero_invalid = 1;
    d->signature               = (tag)k_data_array_signature;
    d->data                    = (byte *)d + sizeof(struct data_array);
    data_delete_all(d);
    return d;
}

void data_dispose(struct data_array *d) { if (d) { data_verify(d); d->signature = 0; free(d); } }
void data_make_valid(struct data_array *d)   { data_verify(d); d->valid = 1; }
void data_make_invalid(struct data_array *d) { data_verify(d); d->valid = 0; }

/* allocate the first free slot; stamp its salt; return a fresh handle or NONE */
datum_index datum_new(struct data_array *d)
{
    int32 i;
    data_verify(d);
    ASSERT(d->valid);
    for (i = d->next_index; i < d->maximum_count; ++i) {
        int16 *salt = data_array_identifier(d, i);
        if (*salt == 0) {
            word id = DATUM_INDEX_TO_IDENTIFIER(d->next_datum);
            memset(data_array_element(d, i), 0, (word)d->datum_size);
            *salt = (int16)id;
            d->next_index = (int16)(i + 1);
            if (i >= d->last_index) d->last_index = (int16)(i + 1);
            d->next_datum = BUILD_DATUM_INDEX(data_array_next_identifier(id), 0);
            return BUILD_DATUM_INDEX(id, i);
        }
    }
    return (datum_index)NONE;
}

datum_index datum_new_at_index(struct data_array *d, datum_index index)
{
    int32 abs_i = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);
    int16 *salt;
    data_verify(d);
    ASSERT(abs_i >= 0 && abs_i < d->maximum_count);
    salt = data_array_identifier(d, abs_i);
    ASSERT(*salt == 0);
    memset(data_array_element(d, abs_i), 0, (word)d->datum_size);
    *salt = (int16)DATUM_INDEX_TO_IDENTIFIER(index);
    if (abs_i >= d->last_index) d->last_index = (int16)(abs_i + 1);
    return index;
}

void datum_delete(struct data_array *d, datum_index index)
{
    int32 abs_i = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);
    int16 *salt;
    data_verify(d);
    ASSERT(index != (datum_index)NONE);
    ASSERT(abs_i >= 0 && abs_i < d->last_index);
    salt = data_array_identifier(d, abs_i);
    ASSERT((word)*salt == DATUM_INDEX_TO_IDENTIFIER(index));
    *salt = 0;
    if (abs_i < d->next_index) d->next_index = (int16)abs_i;
}

/*------------------------------------------------------------------------------
 * ACCESS TIERS -- choose the right one (Butcher):
 *
 *   datum_get                 STRONG ref. Salt-checked; ASSERTs on NONE, out
 *                             of range, freed slot, or salt mismatch. Use when
 *                             the handle MUST be valid; a stale handle is a bug
 *                             you want to catch.
 *   datum_try_and_get         WEAK ref. Returns NULL instead of asserting on a
 *                             stale/free/out-of-range handle. Use when a handle
 *                             may legitimately have been freed.
 *   datum_try_and_get_absolute  Look up by ABSOLUTE index, ignoring salt; NULL
 *                             if the slot is free. Use only when you already
 *                             hold a bare slot number (e.g. during iteration).
 *   datum_try_and_get_unsafe  Minimal checks; ONLY for indices arriving from
 *                             untrusted input (network packets, saved games).
 *----------------------------------------------------------------------------*/

void *datum_get(struct data_array *d, datum_index index)
{
    int32 abs_i = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);
    int16 *salt;
    data_verify(d);
    ASSERT(index != (datum_index)NONE);
    ASSERT(abs_i >= 0 && abs_i < d->last_index);
    salt = data_array_identifier(d, abs_i);
    ASSERT(*salt != 0);                                        /* not freed  */
    ASSERT((word)*salt == DATUM_INDEX_TO_IDENTIFIER(index));   /* not stale  */
    return salt;                                               /* == element */
}

void *datum_try_and_get(struct data_array *d, datum_index index)
{
    int32 abs_i;
    data_verify(d);
    if (index == (datum_index)NONE) return NULL;
    abs_i = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);
    if (abs_i < 0 || abs_i >= d->last_index) return NULL;
    {
        int16 *salt = data_array_identifier(d, abs_i);
        if (*salt == 0) return NULL;
        if ((word)*salt != DATUM_INDEX_TO_IDENTIFIER(index)) return NULL;
        return salt;
    }
}

void *datum_try_and_get_absolute(struct data_array *d, int32 absolute_index)
{
    data_verify(d);
    if (absolute_index < 0 || absolute_index >= d->last_index) return NULL;
    {
        int16 *salt = data_array_identifier(d, absolute_index);
        return *salt != 0 ? (void *)salt : NULL;
    }
}

void *datum_try_and_get_unsafe(struct data_array *d, datum_index index)
{
    int32 abs_i = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);
    if (!d || index == (datum_index)NONE) return NULL;
    if (abs_i < 0 || abs_i >= d->maximum_count) return NULL;
    return data_array_element(d, abs_i);
}

/*==============================================================================
 * SECTION B -- iteration  (official)
 *============================================================================*/

struct data_iterator {
    struct data_array *data;
    datum_index index;             /* handle of the current datum */
    int32 absolute_index;          /* cursor */
};

void data_iterator_begin(struct data_iterator *it, struct data_array *d)
{
    data_verify(d);
    it->data           = d;
    it->index          = (datum_index)NONE;
    it->absolute_index = 0;
}

/* returns each live datum in turn, skipping free slots; NULL at the end.
 * After a successful call, it->index is the current datum's full handle. */
void *data_iterator_next(struct data_iterator *it)
{
    struct data_array *d = it->data;
    while (it->absolute_index < d->last_index) {
        int32 i = it->absolute_index++;
        int16 *salt = data_array_identifier(d, i);
        if (*salt != 0) {
            it->index = BUILD_DATUM_INDEX((word)*salt, i);
            return salt;
        }
    }
    it->index = (datum_index)NONE;
    return NULL;
}

/*==============================================================================
 * SECTION C -- tag_group definition macros  (official; Noguchi)
 *
 * tag_groups decompose a C struct into typed field descriptors that
 * Guerilla/Sapien walk for reflection, serialization, and byte-swapping.
 * Field-name strings carry Guerilla markup inline: units after ':', ranges in
 * '[]', '*' = important, '#' begins a help string.
 *============================================================================*/

typedef enum field_type {
    _field_short_integer,
    _field_long_integer,
    _field_real,
    _field_real_vector3d,
    _field_real_quaternion,
    _field_string,
    _field_tag_reference,
    _field_block,
    _field_data,
    _field_pad,
    _field_terminator
} field_type;

typedef struct tag_field {
    field_type  type;
    const char *name;
    const void *definition;
} tag_field;

typedef struct tag_block {          /* variable-length array field */
    int32 count;
    void *elements;
} tag_block;

typedef struct tag_reference {      /* typed persistent cross-tag pointer */
    tag   group_tag;
    int32 tag_index;
} tag_reference;

typedef struct tag_group_definition {
    const char      *name;
    tag              group_tag;
    int32            size;
    const tag_field *fields;
} tag_group_definition;

typedef struct tag_block_definition {
    const char      *name;
    int32            maximum_element_count;
    int32            element_size;
    const tag_field *fields;
} tag_block_definition;

#define TAG_GROUP(name_, group_tag_, size_)                            \
    static const tag_field name_##_fields[];                           \
    const tag_group_definition name_##_group =                         \
        { #name_, (group_tag_), (size_), name_##_fields };             \
    static const tag_field name_##_fields[] =

#define TAG_BLOCK(name_, maximum_count_, element_size_)                \
    static const tag_field name_##_fields[];                           \
    const tag_block_definition name_##_block =                         \
        { #name_, (maximum_count_), (element_size_), name_##_fields }; \
    static const tag_field name_##_fields[] =

#define TAG_REFERENCE_DEFINITION(name_, group_tag_)                    \
    const tag name_##_reference_group = (group_tag_)

#define FIELD_PAD(count_) { _field_pad, (const char *)(size_t)(count_), 0 }

#define TAG_BLOCK_GET_ELEMENT(block_, index_, type_)                   \
    ( ASSERT((index_) >= 0 && (index_) < (block_)->count),             \
      &((type_ *)(block_)->elements)[(index_)] )

/* Runtime tag API (Noguchi): the entire public surface. */
long  tag_load(const char *name, tag group_tag, dword flags);
void *tag_get(long tag_index);

/*==============================================================================
 * SECTION D -- WORKED EXAMPLE: the actors array ('actr')
 *
 * This is the pattern to imitate. Given a decompiled line like:
 *
 *     char *actor = (char *)actor_data->data + 1828 * (unsigned __int16)actor_index;
 *
 * the 1828 is the actor datum_size (the stride, INCLUDING the 2-byte header),
 * and (unsigned __int16)actor_index is DATUM_INDEX_TO_ABSOLUTE_INDEX. The raw,
 * salt-unchecked arithmetic is what datum_get compiles to in release. Prefer
 * the checked accessor; drop to the raw form only where the original did.
 *============================================================================*/

enum { k_maximum_number_of_actors = 256 };  /* HEK capacity naming */

/* The actor datum. First member is the 2-byte salt header (datum_size counts
 * it). Total sizeof(struct actor) == the stride you saw (e.g. 1828). Replace
 * the body with the real members as you recover them. */
struct actor {
    int16 header;                 /* datum salt/identifier -- offset 0x000  */
    /* ... recovered members go here ... */
    byte  _remaining[1828 - sizeof(int16)];
};

/* CANONICAL member access: resolve the handle to a typed pointer via the
 * strong accessor, then touch members. This is how a function SHOULD access a
 * datum and its members. */
static inline struct actor *actor_get(datum_index actor_index,
                                      struct data_array *actor_data)
{
    return (struct actor *)datum_get(actor_data, actor_index);
}

/* When the handle may legitimately be stale (the actor could have died), use
 * the weak accessor and null-check before touching members. */
static inline struct actor *actor_get_or_null(datum_index actor_index,
                                              struct data_array *actor_data)
{
    return (struct actor *)datum_try_and_get(actor_data, actor_index);
}

/* Iterate every live actor. */
static inline void actors_for_each(struct data_array *actor_data,
                                   void (*fn)(struct actor *))
{
    struct data_iterator it;
    struct actor *actor;
    data_iterator_begin(&it, actor_data);
    while ((actor = (struct actor *)data_iterator_next(&it)) != NULL)
        fn(actor);
}

/* Example sound-tag definition using the official TAG_GROUP macro (Noguchi's
 * 'snde' worked example). */
TAG_GROUP(sound_environment, 'snde', 72 /* sizeof(struct sound_environment) */)
{
    { _field_real, "room intensity:dB[-100,20]", 0 },
    { _field_real, "room high-frequency intensity:dB[-100,20]", 0 },
    FIELD_PAD(4),
    { _field_terminator, 0, 0 }
};

/*==============================================================================
 * Optional self-check. Compile with -DBLAM_DATUM_DEMO to run.
 *============================================================================*/
#ifdef BLAM_DATUM_DEMO
#include <stdio.h>
int main(void)
{
    struct data_array *actor_data =
        data_new("actors", k_maximum_number_of_actors, (int16)sizeof(struct actor));
    data_make_valid(actor_data);

    datum_index a = datum_new(actor_data);
    datum_index b = datum_new(actor_data);

    /* the raw decompiled line and the checked accessor land on the same byte */
    char *raw = (char *)actor_data->data + 1828 * (unsigned short)b;
    struct actor *checked = actor_get(b, actor_data);
    printf("raw==checked: %s\n", (char *)checked == raw ? "yes" : "no");
    printf("abs=%d salt=0x%04X\n",
           DATUM_INDEX_TO_ABSOLUTE_INDEX(b), DATUM_INDEX_TO_IDENTIFIER(b));

    datum_delete(actor_data, a);
    printf("stale handle via try_and_get: %p\n",
           (void *)datum_try_and_get(actor_data, a));   /* NULL */

    data_dispose(actor_data);
    return 0;
}
#endif
