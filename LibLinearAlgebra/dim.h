#include "nmath_global.h"

#pragma once

NMATH_NAMESPACE_BEG

class dim 
{
public:
	enum dimAxis { X = 0, Y, Z };

private:
	dimAxis mAxis;

public:
	dim(dimAxis axis);
	dim(char axis);

	dimAxis& axis();
	const dimAxis& axis() const;

	operator uint8_t() { return (uint8_t)this->mAxis; }
};

NMATH_NAMESPACE_END
