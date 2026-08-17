/* ?convertAttributesToProperties@hkdAssetProcessingUtil@@SAXABV?$hkArray@UhkxAttribute@@@@PBDPAVhkdProperties@@@Z @0x83839BD0 */
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkxAttribute.h"
#include "../headers/havok/hkdProperties.h"
#include "../headers/havok/hkClass.h"
#include "../headers/havok/hkError.h"

/* Boundary callees (2-level descent). */
extern const char *hkClass_getName(const hkClass *cls);   /* hkClass::getName */
extern int hkString_strCmp(const char *a, const char *b); /* hkString::strCmp */

#define ASSET_UTIL_CPP "D:\\Projects\\code\\common\\lib_3dpart\\phys\\hk_include\\Destruction\\Destruction\\AssetProcessing\\hkdAssetProcessingUtil.cpp"

/* hkdAssetProcessingUtil::convertAttributesToProperties — translate each sparsely
   animated custom attribute of an exported object into a runtime hkdProperty.
   Only BOOL / INT / ENUM / FLOAT valued attributes are supported; anything else
   emits a warning and is skipped. */
void hkdAssetProcessingUtil_convertAttributesToProperties(
    const hkArray<hkxAttribute> *sourceAttributesArray,
    const char *objectName,
    hkdProperties *destProperties)
{
    int i;

    for (i = 0; i < sourceAttributesArray->m_size; ++i)
    {
        hkxAttribute *attr = &sourceAttributesArray->m_data[i];
        unsigned int key;
        const char *valueClassName;

        hkpProperty_mapStringToKey(attr->m_name, &key);
        valueClassName = hkClass_getName(attr->m_value.m_class);

        if (!hkString_strCmp(valueClassName, "hkxSparselyAnimatedBool"))
        {
            /* Bool: read the animated bool's current value straight from the object. */
            hkdProperties_addProperty(destProperties, key,
                (hkpPropertyValue *)(**(unsigned char **)attr->m_value.m_object != 0));
        }
        else if (!hkString_strCmp(valueClassName, "hkxSparselyAnimatedInt")
                 || !hkString_strCmp(valueClassName, "hkxSparselyAnimatedEnum"))
        {
            /* Int / Enum: the property value is the doubly-indirected first slot. */
            hkdProperties_addProperty(destProperties, key, **(hkpPropertyValue ***)attr->m_value.m_object);
        }
        else if (!hkString_strCmp(valueClassName, "hkxAnimatedFloat"))
        {
            hkdProperties_addProperty(destProperties, key, **(hkpPropertyValue ***)attr->m_value.m_object);
        }
        else
        {
            char buffer[512];
            hkErrStream es;
            hkErrStream_construct(&es, buffer, 512);
            hkOstream_insertString(
                hkOstream_insertString(
                    hkOstream_insertString(
                        hkOstream_insertString(&es.base, objectName),
                        ": Custom Properties only support BOOL, ENUM, INT and FLOAT values. Found '"),
                    valueClassName),
                "'.");
            hkError_message(hkError_s_instance, MESSAGE_WARNING, -1413815247, buffer, ASSET_UTIL_CPP, 2825);
            hkOstream_destruct(&es.base);
        }
    }
}
