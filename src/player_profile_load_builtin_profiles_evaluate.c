/* player_profile_load_builtin_profiles_evaluate @0x8372E450 — HaloScript builtin wrapper for
 * player_profile_load_builtin_profiles. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void player_profile_load_builtin_profiles(const char *matched_name);

void player_profile_load_builtin_profiles_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        player_profile_load_builtin_profiles((const char *)arguments[0]);
        hs_return(thread_index, 0);
    }
}
