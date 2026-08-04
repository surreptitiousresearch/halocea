#ifndef AI_INDEX_PLATOON_ITERATOR_H
#define AI_INDEX_PLATOON_ITERATOR_H

/* Cursor for walking the platoons selected by an AI index within one encounter. encounter_index == -1 marks an
 * empty/invalid iterator; otherwise platoon_index..last_platoon_index (inclusive) is the range to visit. */
typedef struct ai_index_platoon_iterator
{
    int encounter_index;       /* 0x00 */
    int platoon_index;         /* 0x04 — current platoon (encounter-relative) */
    int last_platoon_index;    /* 0x08 — inclusive end */
} ai_index_platoon_iterator;

#endif /* AI_INDEX_PLATOON_ITERATOR_H */
