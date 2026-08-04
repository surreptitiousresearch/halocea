#ifndef AI_INDEX_SCOPE_H
#define AI_INDEX_SCOPE_H
/* AI-index scope selector — the top two bits (ai_index >> 30) of a packed ai_index choose which
 * sub-scope the low bits address. DB anonymous enum _9800399749B032C5378724B78DD01ED9.
 * Scope value 3 is reserved/invalid and carries no name in the DB. */
enum ai_index_scope
{
    _ai_index_encounter = 0,
    _ai_index_platoon   = 1,
    _ai_index_squad     = 2,
};
#endif /* AI_INDEX_SCOPE_H */
