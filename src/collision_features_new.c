/* collision_features_new @0x838040E0 — reset a feature list to empty (all three feature-kind counts zero). */

#include "headers/collision_feature_list.h"
#include "headers/collision_feature.h"

void collision_features_new(collision_feature_list *features)
{
    features->count[_collision_feature_sphere] = 0;     /* the store clears count[0]+count[1] as one dword */
    features->count[_collision_feature_cylinder] = 0;
    features->count[_collision_feature_prism] = 0;
}
