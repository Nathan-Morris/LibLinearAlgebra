#include "dim.h"

NMATH_NAMESPACE_BEG

dim::dim(dimAxis axis) : mAxis(axis) { }

dim::dim(char axis) : dim((dimAxis)('X' - axis)) { }

dim::dimAxis& dim::axis() { return this->mAxis; }

const dim::dimAxis& dim::axis() const { return this->mAxis; }

NMATH_NAMESPACE_END
