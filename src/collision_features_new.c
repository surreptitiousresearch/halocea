/* collision_features_new @0x838040E0 — reset a feature list to empty (all three feature-kind counts zero). */

#include "headers/collision_feature_list.h"

void collision_features_new(collision_feature_list *features)
{
    features->count[0] = 0;     /* the store clears count[0]+count[1] as one dword */
    features->count[1] = 0;
    features->count[2] = 0;
}
