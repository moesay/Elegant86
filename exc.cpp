#include "exc.h"

const char* InvalidPointer::what() const throw() {
    return "Invalid Pointer";
}

const char* InvalidSegmentOverridePrefix::what() const throw() {
    return "Invalid Segment Override Prefix";
}
