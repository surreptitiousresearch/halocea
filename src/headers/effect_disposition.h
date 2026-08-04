#ifndef EFFECT_DISPOSITION_H
#define EFFECT_DISPOSITION_H

/* effect_disposition — the violence disposition gate on an effect part / particle definition
 * (effect_part_definition.disposition, effect_particles_definition.disposition). A part is skipped
 * unless its disposition matches the effect's violence state: a nonviolent effect keeps only
 * _effect_disposition_violent parts, a violent effect keeps only _effect_disposition_nonviolent ones.
 * DB-verified: types_enum_values _CCD8A3BE785BE5FDDB0780D99BD71280 carries these names verbatim. */
enum effect_disposition
{
    _effect_disposition_agnostic   = 0,
    _effect_disposition_violent    = 1,
    _effect_disposition_nonviolent = 2,
    NUMBER_OF_EFFECT_DISPOSITIONS  = 3,
};

#endif /* EFFECT_DISPOSITION_H */
