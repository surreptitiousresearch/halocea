/* hs_evaluate_by_name @ 0x8372FEB8 — look up an hs (HaloScript) script by name and evaluate its
 * root expression. Returns 1 if the script existed, 0 otherwise. The script's root expression
 * index lives at dword offset 9 of its 23-dword hs_scripts entry. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_script.h"

extern int16_t hs_find_script_by_name(const char *name);
extern int hs_runtime_evaluate(int expression_index);

int hs_evaluate_by_name(const char *script_name)
{
    int16_t script = hs_find_script_by_name(script_name);
    if ( script == -1 )
        return 0;

    hs_runtime_evaluate(((hs_script *)global_scenario->hs_scripts.address)[script].root_expression_index);
    return 1;
}
