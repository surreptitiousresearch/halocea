/* multiplayer_settings_select_list_update_item @0x8377EF18 — refresh one row of the multiplayer-settings
 * variant-select list to reflect the given game variant. The row widget has three relevant descendants: the
 * first child holds the variant's human-readable description text, its sibling is the icon widget whose
 * animation frame selects the game-engine glyph, and that sibling's sibling holds the engine/mode label string;
 * a fourth widget (the description container) is shown only for the "built-in with a canned description" case.
 *
 * With a variant: copy human_readable_game_description into the first child (<=127 wchars), set the icon frame
 * from the game-engine index (assault/CTF/... -> frames 0..4), and fill the label box. If the variant carries a
 * built-in description id (flags bit 0), the label is that description string ((flags>>8)+10 in the
 * game_variant_descriptions table) and the description container is shown; otherwise the label is the engine's
 * teams/no-teams mode name (index engine*2-2 + (teams?1:0) offset table) from the same table. With a null
 * variant: clear both boxes, set the icon to frame 5 (the "empty" glyph), and, if the profile-description-labels
 * string list is loaded, show its "no variant" string (index 5).
 *
 * DEVIATION: variant->flags is the 16-bit game_variant.flags @0x94 (disasm 0x8377EFB4: lhz 0x94); flags&1 is
 * the built-in-description bit, flags>>8 the description id. The decompiler's per-case goto LABEL_51 tail is
 * folded into a shared string-fill after each switch. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/game_variant.h"
#include "headers/game_engine_type.h"

extern int tag_loaded(uint32_t group_tag, const char *name);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);

void multiplayer_settings_select_list_update_item(widget_instance *item, game_variant *variant)
{
    int descriptions_tag = tag_loaded(0x75737472u, "ui\\shell\\strings\\game_variant_descriptions");  /* 'ustr' */
    widget_instance *description_box = item->children;
    widget_instance *icon = description_box->next;
    widget_instance *label_box = icon->next;
    widget_instance *description_container = label_box->next;

    description_container->visible = 0;

    if ( !variant )
    {
        unsigned __int16 *cleared = ui_widget_realloc(
                description_box->parameters.text_box_parameters.text, 0x100u);
        description_box->parameters.text_box_parameters.text = cleared;
        if ( cleared )
            *cleared = 0;
        icon->animation_data.current_frame_index = 5;
        unsigned __int16 *label = ui_widget_realloc(
                label_box->parameters.text_box_parameters.text, 0x200u);
        label_box->parameters.text_box_parameters.text = label;
        if ( label )
        {
            int labels_tag = tag_loaded(0x75737472u,
                    "ui\\shell\\main_menu\\player_profiles_select\\profile_description_labels");
            *label_box->parameters.text_box_parameters.text = 0;
            if ( labels_tag != -1 )
            {
                unsigned __int16 *string = unicode_string_list_get_string(labels_tag, 5);
                ustrncpy(label_box->parameters.text_box_parameters.text, (const wchar_t *)string, 0xFFu);
                label_box->parameters.text_box_parameters.text[255] = 0;
            }
        }
        return;
    }

    unsigned __int16 *description = ui_widget_realloc(
            description_box->parameters.text_box_parameters.text, 0x100u);
    description_box->parameters.text_box_parameters.text = description;
    if ( description )
    {
        ustrncpy(description, variant->human_readable_game_description, 0x7Fu);
        description_box->parameters.text_box_parameters.text[127] = 0;
    }

    icon->animation_data.current_frame_index = 5;
    unsigned __int16 *label = ui_widget_realloc(
            label_box->parameters.text_box_parameters.text, 0x200u);
    label_box->parameters.text_box_parameters.text = label;
    if ( label )
        *label = 0;

    unsigned int flags = variant->flags;
    unsigned int engine_minus_one = variant->game_engine_index - 1;

    if ( (flags & 1) != 0 )
    {
        int description_id = (flags >> 8) + 10;
        if ( engine_minus_one <= 4 )
        {
            switch ( variant->game_engine_index )
            {
                case game_engine_slayer:  icon->animation_data.current_frame_index = 2; break;
                case game_engine_oddball:  icon->animation_data.current_frame_index = 3; break;
                case game_engine_king:  icon->animation_data.current_frame_index = 1; break;
                case game_engine_ctf:  icon->animation_data.current_frame_index = 0; break;
                default: /* game_engine_race */ icon->animation_data.current_frame_index = 4; break;
            }
        }
        if ( descriptions_tag != -1 && label_box->parameters.text_box_parameters.text )
        {
            unsigned __int16 *string = unicode_string_list_get_string(descriptions_tag, description_id);
            ustrncpy(label_box->parameters.text_box_parameters.text, (const wchar_t *)string, 0xFFu);
            label_box->parameters.text_box_parameters.text[255] = 0;
        }
        description_container->visible = 1;
        return;
    }

    if ( engine_minus_one > 4 )
        return;

    __int16 mode_string_index;
    switch ( variant->game_engine_index )
    {
        case game_engine_slayer:
            icon->animation_data.current_frame_index = 2;
            mode_string_index = variant->universal_variant.teams == 1 ? 3 : 2;
            break;
        case game_engine_oddball:
            icon->animation_data.current_frame_index = 3;
            mode_string_index = variant->universal_variant.teams == 1 ? 5 : 4;
            break;
        case game_engine_king:
            icon->animation_data.current_frame_index = 1;
            mode_string_index = variant->universal_variant.teams == 1 ? 7 : 6;
            break;
        case game_engine_ctf:
            icon->animation_data.current_frame_index = 0;
            mode_string_index = variant->universal_variant.teams == 1;
            break;
        default: /* game_engine_race */
            icon->animation_data.current_frame_index = 4;
            mode_string_index = variant->universal_variant.teams == 1 ? 9 : 8;
            break;
    }

    if ( descriptions_tag == -1 || !label_box->parameters.text_box_parameters.text )
        return;

    unsigned __int16 *string = unicode_string_list_get_string(descriptions_tag, mode_string_index);
    ustrncpy(label_box->parameters.text_box_parameters.text, (const wchar_t *)string, 0xFFu);
    label_box->parameters.text_box_parameters.text[255] = 0;
}
