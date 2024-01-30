#pragma once
#include "../../core/displacement.h"

#define MAX_STUDIO_BONES 1024

class CHitbox
{
public:
	__int32	 m_unk1;
	__int32	 m_unk2;
	__int32	 m_unk3;
	__int32	 m_unk4;
	__int32	 m_unk5;
	__int32	 m_unk6;
	__int32	 m_unk7;
	__int32	 m_unk8;
	Vector	 m_vMinBounds;		 // 0x0020
	Vector	 m_vMaxBounds;		 // 0x002C
	char* m_name;			 // 0x0038
	char* m_sSurfaceProperty; // 0x0040
	char* m_sBoneName;		 // 0x0048
	float	 m_flShapeRadius;	 // 0x0050
	uint32_t m_nBoneNameHash;	 // 0x0054
	__int32	 m_nGroupId;		 // 0x0058
	__int32	 m_cRenderColor;	 // 0x005C
	__int16	 m_nHitBoxIndex;	 // 0x0060
	__int16	 m_nShapeType;		 // 0x0062
	char	 pad_0x0064[ 0xC ];	 // 0x0064

}; // Size=0x0070

class CHitboxSet
{
public:
	char	  pad_0x0000[ 0x20 ]; // 0x0000
	uint32_t  m_nNameHash;		// 0x0020
	char	  pad_0x0024[ 0x4 ];	// 0x0024
	__int32	  m_nHitboxCount;	// 0x0028
	char	  pad_0x002C[ 0x4 ];	// 0x002C
	CHitbox* m_arrHitbox;			// 0x0030
	char	  pad_0x0038[ 0x18 ]; // 0x0038
};

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