/* hcex_cine_obj_term @0x823B70B8 — remove a Blam object id from the cinematic-object list. Walks
 * hcexObjectsCine (a dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8> keyed string -> object id) back-to-front
 * and erases every pair whose val equals the id. The dsVECTOR template ops are ws-engine C++ (extern). */

#include "../headers/ds_vector_generic.h"
#include "../headers/hcex/hcex_cine_pair.h"

extern ds_vector_generic hcexObjectsCine;

extern hcex_cine_pair *dsVECTOR_dsPAIR_dsTSTRING_char_int_8__operatorSubscript(ds_vector_generic *vector, int position);
extern void            dsVECTOR_dsPAIR_dsTSTRING_char_int_8__Erase(ds_vector_generic *vector, int position, int count);

void hcex_cine_obj_term(int id)
{
    for ( int i = hcexObjectsCine.nElem - 1; i >= 0; --i )
    {
        if ( dsVECTOR_dsPAIR_dsTSTRING_char_int_8__operatorSubscript(&hcexObjectsCine, i)->val == id )
            dsVECTOR_dsPAIR_dsTSTRING_char_int_8__Erase(&hcexObjectsCine, i, 1);
    }
}
