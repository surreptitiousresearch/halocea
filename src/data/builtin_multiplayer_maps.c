/* builtin_multiplayer_maps @ 0x8417EDE0, 228 bytes = 19 x buildin_multiplayer_map_s (12 bytes each).
   Build-time table source for map_list_initialize_known_list. Each record holds a
   map_index, a char* name relocation into .rdata, and the original_xbox_map flag.
   Names recovered from the .rdata strings the pointers target (big-endian image).
   original_xbox_map is 1 for the 14 original Xbox maps (indices 0..13) and 0 for
   the five PC/CE-added maps (indices 14..18). */
#include "../headers/buildin_multiplayer_map_s.h"

buildin_multiplayer_map_s builtin_multiplayer_maps[19] = {
    {  0, "levels\\test\\beavercreek\\beavercreek",       1 },
    {  1, "levels\\test\\sidewinder\\sidewinder",         1 },
    {  2, "levels\\test\\damnation\\damnation",           1 },
    {  3, "levels\\test\\ratrace\\ratrace",               1 },
    {  4, "levels\\test\\prisoner\\prisoner",             1 },
    {  5, "levels\\test\\hangemhigh\\hangemhigh",         1 },
    {  6, "levels\\test\\chillout\\chillout",             1 },
    {  7, "levels\\test\\carousel\\carousel",             1 },
    {  8, "levels\\test\\boardingaction\\boardingaction", 1 },
    {  9, "levels\\test\\bloodgulch\\bloodgulch",         1 },
    { 10, "levels\\test\\wizard\\wizard",                 1 },
    { 11, "levels\\test\\putput\\putput",                 1 },
    { 12, "levels\\test\\longest\\longest",               1 },
    { 13, "levels\\test\\icefields\\icefields",           1 },
    { 14, "levels\\test\\deathisland\\deathisland",       0 },
    { 15, "levels\\test\\dangercanyon\\dangercanyon",     0 },
    { 16, "levels\\test\\infinity\\infinity",             0 },
    { 17, "levels\\test\\timberland\\timberland",         0 },
    { 18, "levels\\test\\gephyrophobia\\gephyrophobia",   0 },
};
