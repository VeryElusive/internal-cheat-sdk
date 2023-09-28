#pragma once

struct Gun_t;



// struct name is assault cube is 'playerent'
struct Player_t {
	void* VTable{ };// 0x0
	Vector m_vecOrigin{ }; // 0x4
	char pad2[ 0x66 ]{ };// 0x10
	int m_iState{ };// 0x76
	char pad3[ 0x18A ]{ };// 0x7A
	bool m_bAttacking{ };// 0x204
	char pad4[ 0x104 ]{ };// 0x205
	int m_iTeam{ };// 0x309
	char pad5[ 0x57 ]{ };// 0x30D
	Gun_t* m_pWeapon{ };// 0x364
};

// struct name in assault cube is 'gun'
struct Gun_t {
	char pad[ 0x8 ]{ };// 0x0
	Player_t* m_pOwner{ };// 0x8
};