/* input_device_defaults_create_builtin_profiles @0x83802838 — find the "devc" (input device) tag whose
 * embedded default profile (device_type == full_profile marks a profile-bearing device tag; the
 * player_profile blob is the tag's profile.address tag_data) is named `name`, and if it isn't already present as a saved player profile, create one
 * from it: allocate a new profile slot, copy the relevant fields from the tag's built-in profile, mark it
 * as a built-in/default profile (flags bits 0x2 and 0x4), and save it. */

#include <wchar.h>
#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/player_profile.h"
#include "headers/tag_iterator.h"
#include "headers/input_device_defaults.h"

extern int ustrcasecmp(const wchar_t *string1, const wchar_t *string2);
extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);
extern int player_profile_new(int16_t local_player_index, const uint16_t *name);
extern uint8_t player_profile_get(int player_profile_index, player_profile *profile);
extern uint8_t copy_relevant_profile_data(int type, player_profile *dst, const player_profile *profile);
extern void player_profile_save(int player_profile_index, player_profile *profile);

void input_device_defaults_create_builtin_profiles(const wchar_t *name)
{
    tag_iterator iterator;
    tag_iterator_new(&iterator, 0x64657663u);   /* "devc" */

    int tag_index = tag_iterator_next(&iterator);
    if ( tag_index == -1 )
        return;

    const player_profile *builtin_profile;
    while ( 1 )
    {
        const input_device_defaults *definition = TAG_GET(const input_device_defaults, tag_index);
        if ( definition->device_type == _input_device_defaults_device_type_full_profile )
        {
            builtin_profile = (const player_profile *)definition->profile.address;
            if ( !ustrcasecmp(name, builtin_profile->player_name) )
                break;
        }

        tag_index = tag_iterator_next(&iterator);
        if ( tag_index == -1 )
            return;
    }

    int profile_index = player_profile_new(0, builtin_profile->player_name);
    if ( profile_index != -1 )
    {
        player_profile profile;
        if ( player_profile_get(profile_index, &profile) )
        {
            if ( copy_relevant_profile_data(2, &profile, builtin_profile) )
            {
                profile.flags |= 6u;
                player_profile_save(profile_index, &profile);
            }
        }
    }
}
