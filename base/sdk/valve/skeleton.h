#pragma once
#include "../../core/displacement.h"

#define MAX_STUDIO_BONES 1024

struct alignas( 16 ) CBoneData {
	Vector m_vecPosition;
	float m_flScale;
	Vector4D m_vecRotation;
};

class CModel {
public:
	unsigned int GetBoneParent( const int index ) {
		return Displacement::GetBoneParent( this, index );
	}

	unsigned int GetBoneFlags( const int index ) {
		return Displacement::GetBoneFlags( this, index );
	}
public:
	std::uint8_t pad_0[ 0x170 ];
	std::int32_t m_iBoneCount;
};

class CModelState {
public:
	std::uint8_t pad_0[ 0x80 ];
	CBoneData* m_pBones;
	std::uint8_t pad_1[ 0x18 ];
	CStrongHandle<CModel> m_hModel;
	void* m_szModelName;//CUtlSymbolLarge
};