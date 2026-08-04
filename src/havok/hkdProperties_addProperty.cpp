#include "../headers/havok/hkdProperties.h"

/* hkArrayUtil::_reserveMore — grow the array by one element's worth of capacity.
   First arg is &array.m_data. Outside this slice. */
extern void hkArrayUtil__reserveMore(void *arrayData, int elementSize); /* hkArrayUtil::_reserveMore */

/* hkdProperties::addProperty @ 0x8384BD48
   Append a (key,value) pair, growing the property array when full. */
void hkdProperties_addProperty(hkdProperties *self, unsigned int key, hkpPropertyValue *value)
{
    hkpProperty *entry;
    int size;

    if (self->m_properties.m_size == (self->m_properties.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&self->m_properties.m_data, 16);

    size = self->m_properties.m_size;
    entry = &self->m_properties.m_data[size];
    self->m_properties.m_size = size + 1;
    entry->m_key = key;
    entry->m_value.m_data = value->m_data;
}
