#ifndef AI_FOLLOW_TARGET_TYPE_H
#define AI_FOLLOW_TARGET_TYPE_H

/* Values for encounter_datum::follow_target_type.
 * DB anonymous enum _3380E72176F2BF9AA97DC011AA683BCE. */
enum ai_follow_target_type
{
    _follow_target_none    = 0,
    _follow_target_players = 1,
    _follow_target_unit    = 2,
    _follow_target_ai      = 3,
    NUMBER_OF_FOLLOW_TARGET_TYPES = 4
};

#endif /* AI_FOLLOW_TARGET_TYPE_H */
