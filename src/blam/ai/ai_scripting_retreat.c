/* ai_scripting_retreat @0x83772A10 — alias for ai_scripting_maneuver (a retreat is a maneuver). */

extern void ai_scripting_maneuver(int ai_index);

void ai_scripting_retreat(int ai_index)
{
    ai_scripting_maneuver(ai_index);
}
