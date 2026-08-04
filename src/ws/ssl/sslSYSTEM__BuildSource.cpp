#include "sslSYSTEM.h"
#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslCLASS.h"
#include "sslCLASS_REF.h"
#include "sslSOURCE.h"
#include "sslSOURCE_REF.h"
#include "sslBREAKPOINT.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"

// Count the number of text lines in a NUL-terminated string; not reversed here.
int CalcNumLines(const char *text); // boundary
// dsTSTRING concatenation (free operators); not reversed here.
template<class T> dsTSTRING<T> operator+(const T *lhs, const dsTSTRING<T> &rhs); // boundary
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const T *rhs); // boundary

// 0x825D1528 — sslSYSTEM::BuildSource. Assemble the concatenated source text of an object's class
// hierarchy (each class prefixed with a "// class <name> ******" banner) and map breakpoint lines.
// Three modes selected by `mode`:
//   0 : write the full text into `src_text`, collect every valid-breakpoint line (banner-adjusted)
//       into `arBPLines`, and every active breakpoint scoped to this object into `arBP`. Returns -1.
//   1 : locate the source whose banner-adjusted valid-breakpoint line equals `line`; set `*src` to it
//       and return the source-local line. Returns -1 on no match.
//   >=2 : find the raw valid-breakpoint line equal to `line` and return its banner-adjusted line.
// Returns int (H mangle) — not an sslERROR by value.
int sslSYSTEM::BuildSource(int class_uid, int obj_uid, int line, int mode, sslSOURCE_REF *src,
                           dsVECTOR<int, 8> *arBPLines, dsVECTOR<int, 8> *arBP, dsTSTRING<char> *src_text)
{
    if (class_uid == -1 || obj_uid == -1)
        return -1;

    sslOBJ_REF obj = this->FindObject(obj_uid);
    if (!obj.pObject)
        return -1;

    // Walk from the object's own class up to the one whose uid matches class_uid.
    sslCLASS_REF cls = obj.GetClass();
    if (class_uid != obj_uid && cls.pClass)
    {
        do
        {
            if (cls.GetUID() == class_uid)
                break;
            cls = cls.GetParent();
        } while (cls.pClass);
    }

    // Collect the class chain from the matched class up to the root.
    dsVECTOR<sslCLASS_REF, 8> chain;
    while (cls.pClass)
    {
        chain.PushBack(cls);
        cls = cls.GetParent();
    }

    dsTSTRING<char> outText;
    outText.pBuffer = nullptr;
    outText.UnsafeInit("", -1, 0);

    int result = -1;

    // Emit each class from the root down to the most-derived.
    for (int ci = chain.nElem - 1; ci >= 0; --ci)
    {
        sslCLASS *c = chain[ci].pClass;

        // This class's source text ("" for a null class).
        dsTSTRING<char> classText;
        if (c)
        {
            classText = c->GetSource().Get();
        }
        else
        {
            classText.pBuffer = nullptr;
            classText.UnsafeInit("", -1, 0);
        }

        // Banner: "\n\n// class <name>   ******\n\n", with "<unnamed>" for a nameless class.
        dsTSTRING<char> className = this->GetClassName(chain[ci]);
        if (!className.pBuffer->strLen)
        {
            dsTSTRING<char> unnamed;
            unnamed.pBuffer = nullptr;
            unnamed.UnsafeInit("<unnamed>", -1, 0);
            className = unnamed;
        }
        outText += "\n\n// class " + className + "   ******\n\n";

        // Line at which this class's source begins in the aggregate.
        int baseLineOffset = CalcNumLines(outText.CStr());
        outText += classText;

        if (!c)
            continue;

        sslSOURCE_REF srcRef = c->GetSource();
        sslSOURCE *source = srcRef.source;
        int numValidBP = source ? source->validBPLines.nElem : 0;
        const int *validBP = source ? source->validBPLines.pData : nullptr;

        bool matched = false;
        for (int bi = 0; bi < numValidBP; ++bi)
        {
            if (mode == 0)
            {
                arBPLines->PushBack(validBP[bi] + baseLineOffset);
            }
            else if (mode == 1)
            {
                if (validBP[bi] + baseLineOffset == line)
                {
                    *src = c->GetSource();
                    result = validBP[bi];
                    matched = true;
                    break;
                }
            }
            else // mode >= 2
            {
                if (validBP[bi] == line)
                {
                    result = validBP[bi] + baseLineOffset;
                    matched = true;
                    break;
                }
            }
        }
        if (matched)
            break;

        // Mode 0: gather the source's active breakpoints scoped to this object.
        if (mode == 0 && source)
        {
            const sslBREAKPOINT *bps = source->Breakpoints.pData;
            for (int k = 0; k < source->Breakpoints.nElem; ++k)
            {
                if (bps[k].object_uid == obj_uid)
                    arBP->PushBack(bps[k].line + baseLineOffset);
            }
        }
    }

    if (mode == 0)
        *src_text = outText;

    return result;
}
