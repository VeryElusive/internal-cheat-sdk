#pragma once
#include "matrix.h"
#include "quaternion.h"

class CTransform
{
public:
	VectorAligned m_vecPosition;
	QuaternionAligned_t m_qOrientation;
};

static_assert( alignof( CTransform ) == 16 );
