#pragma once
#include "../../core/displacement.h"
#include "../utlstring.h"

#define MAX_STUDIO_BONES 1024

class CHitbox
{
public:
	CUtlString m_strName{ };//0x0
	Vector m_vecMins{ };//0x18
	Vector m_vecMaxs{ };//0x24
	MEM_PAD( 0x4 );//0x30
	unsigned int m_nBoneNameHash{ };//0x34
	MEM_PAD( 0x38 );// 0x38

	__int64 GetBoneIndex( void* model ) {
		return Displacement::GetBoneIndexFromHitbox( reinterpret_cast< void* > ( reinterpret_cast< std::uintptr_t >( model ) + 0x128 ), m_nBoneNameHash );
	}

}; // Size=0x70

class CHitboxSet
{
public:
	MEM_PAD( 0x10 ); // 0x00

	int m_nHitboxCount{ }; // 0x10 (E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? 48 8B CF E8 ? ? ? ? 85 C0 75 2D 49 8B CE)
	MEM_PAD( 0x4 );//0x14
	CHitbox* m_arrHitboxs{ };// 0x14

	//CUtlVector<CHitbox> m_HitBoxes;// 0x10
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
	//std::uint8_t pad_0[ 0x170 ];
	///std::int32_t m_iBoneCount;

	const char* m_szName; // 0x8
	MEM_PAD( 0x4 ); // 0x10
	int m_nFlags; // 0x14
	Vector m_vecHullMin; // 0x18
	Vector m_vecHullMax; // 0x24
	Vector m_vecViewMin; // 0x30
	Vector m_vecViewMax; // 0x3C
	MEM_PAD( 0x28 ); // 0x48
	int m_nRenderMeshCount; // 0x70
	MEM_PAD( 0x4 ); // 0x74
	void* m_pRenderMesh; // 0x78
	MEM_PAD( 0x50 ); // 0x80
	void* m_pPhysModel; // 0xD0
	int m_nAnimationGroupResourceCount; // 0xD8
	MEM_PAD( 0x4 ); // 0xDC
	void* m_pAnimationGroupResource; // 0xE0
	MEM_PAD( 0x8 ); // 0xE8
	int m_nMeshesCount; // 0xF0
	MEM_PAD( 0x4 ); // 0xF4
	void* m_pMeshes; // 0xF8
	MEM_PAD( 0x28 ); // 0x100
	void* N00000135; // 0x128
	MEM_PAD( 0x28 ); // 0x130
	int m_nNamesCount; // 0x158
	MEM_PAD( 0x4 ); // 0x15C
	const char** m_pNames; // 0x160
	MEM_PAD( 0x8 ); // 0x168
	int m_iBoneCount; // 0x170
	MEM_PAD( 0x4 ); // 0x174
	uint16_t* m_pElementIndex; // 0x178
	MEM_PAD( 0x8 ); // 0x180
	int N00000141; // 0x188
	MEM_PAD( 0x4 ); // 0x18C
	void* N00000142; // 0x190
	MEM_PAD( 0x8 ); // 0x198
	int N00000144; // 0x1A0
	MEM_PAD( 0x4 ); // 0x1A4
	void* N00000145; // 0x1A8
	MEM_PAD( 0x8 ); // 0x1B0
	int m_nPosCount; // 0x1B8
	MEM_PAD( 0x4 ); // 0x1BC
	Vector* m_pPos; // 0x1C0
	MEM_PAD( 0x8 ); // 0x1C8
	int m_nQuatCount; // 0x1D0
	MEM_PAD( 0x4 ); // 0x1D4
	Quaternion* m_pQuat; // 0x1D8
	MEM_PAD( 0x8 ); // 0x1E0
	int m_nScaleCount; // 0x1E8
	MEM_PAD( 0x4 ); // 0x1EC
	float* m_pScale; // 0x1F0
	MEM_PAD( 0x8 ); // 0x1F8
	int N00000150; // 0x200
	MEM_PAD( 0x4 ); // 0x204
	void* N00000151; // 0x208
	MEM_PAD( 0x8 ); // 0x210
	int m_nSomeCount2; // 0x218
	MEM_PAD( 0x4 ); // 0x21C
	uint16_t* m_pSomeArray2; // 0x220
	MEM_PAD( 0x8 ); // 0x228
	int m_nSomeCount1; // 0x230
	MEM_PAD( 0x4 ); // 0x234
	uint16_t* m_pSomeArray1; // 0x238
	MEM_PAD( 0x20 ); // 0x240
	int N0000015C; // 0x260
	MEM_PAD( 0x4 ); // 0x264
	void* N0000015D; // 0x268
	MEM_PAD( 0x8 ); // 0x270
	int N0000015F; // 0x278
	MEM_PAD( 0x4 ); // 0x27C
	void* N00000160; // 0x280
	MEM_PAD( 0x40 ); // 0x288
	int N00000169; // 0x2C8
	MEM_PAD( 0x4 ); // 0x2CC
	void* N0000016A; // 0x2D0
	MEM_PAD( 0x8 ); // 0x2D8
	int m_nSomeCount3; // 0x2E0
	MEM_PAD( 0x4 ); // 0x2E4
	uint16_t* m_pSomeArray3; // 0x2E8
	MEM_PAD( 0x58 ); // 0x2F0
	void* N00000179; // 0x348
	MEM_PAD( 0x8 ); // 0x350
	int N0000017B; // 0x358
	MEM_PAD( 0x4 ); // 0x35C
	void* N0000017C; // 0x360
	MEM_PAD( 0x70 ); // 0x368
	int N0000018B; // 0x3D8
	MEM_PAD( 0x4 ); // 0x3DC
	void* N0000018C; // 0x3E0
	MEM_PAD( 0x30 ); // 0x3E8
	int N00000193; // 0x418
	MEM_PAD( 0x4 ); // 0x41C
	void* N00000194; // 0x420
};

class CModelState {
public:
	MEM_PAD( 0x80 );//0x0
	CBoneData* m_pBones;//0x80 found inside IterateBonesAndReturnBoneCount
	MEM_PAD( 0x18 );//0x84
	CStrongHandle<CModel> m_hModel;//0x9C
	void* m_pModel;//0xA0 CUtlSymbolLarge
};