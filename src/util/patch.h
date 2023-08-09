#ifndef _PATCH_H
#define _PATCH_H

#include "../structure/LIR.h"

static void doPatch(LIR::PatchList* tList, std::string label) {
    for (; tList; tList = tList->tail) *(tList->head) = label;
}
static LIR::PatchList* joinPatch(LIR::PatchList* first, LIR::PatchList* second) {
    if (!first) return second;
    LIR::PatchList* tmp = first;
    for (; tmp->tail; tmp = tmp->tail)
        ;
    tmp->tail = second;
    return first;
}

#endif