#include "label.h"

// gen global var label (str)
std::string newGloablVarLabel() {
    return TempVarLabelGenerator::Instance().newGloablVarLabel();
}

// gen label (for jump ...) (str)
std::string newLabel() {
    return TempVarLabelGenerator::Instance().newLabel();
}


static int tempVarID = 1000;

// gen local var id
int newLocalTempVarID() {
  return tempVarID++;
}