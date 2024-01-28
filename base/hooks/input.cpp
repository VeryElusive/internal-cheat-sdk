#include "../havoc.h"
#include "../core/hooks.h"
#include "../menu/menu.h"
#include "../sdk/valve/entity.h"
#include "../sdk/valve/interfaces/ienginecvar.h"
#include "../features/misc/movement.h"
#include "../features/rage/antiaim.h"
#include "../features/rage/lag_record.h"
#include "../core/config.h"

bool __fastcall Hooks::hkMouseInputEnabled( void* rcx ) {
	const auto og{ MouseInputEnabled.Original<decltype( &hkMouseInputEnabled )>( ) };

	return Menu::m_bOpened ? false : og( rcx );
}

bool __fastcall Hooks::hkCreateMove( void* rcx, unsigned int edx, std::int64_t a3, bool morePasses ) {
	const auto og{ CreateMove.Original<decltype( &hkCreateMove )>( ) };

	auto cmd{ Interfaces::Input->GetUserCmd( ) };

	const auto result{ og( rcx, edx, a3, morePasses ) };

	ctx.GetLocal( );
	if ( !ctx.m_pLocal 
		|| !ctx.m_pLocal->m_bPawnIsAlive( ) )
		return result;

	if ( !cmd->cmd.pBase )
		return result;

	const auto localPawn{ Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>( ctx.m_pLocal->m_hPawn( ) ) };
	if ( !localPawn )
		return result;

	if ( !morePasses ) {
		Features::LagCompensation.Main( false );

		Features::Movement.Main( localPawn, cmd );
	}

	Features::AntiAim.Main( localPawn, cmd, !morePasses );

	ctx.m_flForwardmove = cmd->cmd.pBase->flForwardMove;
	ctx.m_flSidemove = cmd->cmd.pBase->flSideMove;

	cmd->cmd.pBase->pViewangles->angValue.NormalizeAngle( ).ClampAngle( );

	if ( !morePasses )
		ctx.m_mapPlayerEntries[ localPawn->GetRefEHandle( ).m_nIndex ].Animations.m_bShouldUpdateBones = true;

	cmd->cmd.pBase->flForwardMove = std::clamp( cmd->cmd.pBase->flForwardMove, -1.f, 1.f );
	cmd->cmd.pBase->flSideMove = std::clamp( cmd->cmd.pBase->flSideMove, -1.f, 1.f );

	//if ( cmd->cmd.nAttack3StartHistoryIndex >= 0 )
	//	printf( "da\n" );

	if ( !morePasses ) {
		/*if ( Input::Pressed( VK_SHIFT ) ) {
			cmd->cmd.nAttackStartHistoryIndex = 0;
			cmd->m_cButtonStates.m_iHeld |= IN_ATTACK;
			cmd->m_cButtonStates.m_iToggle |= IN_ATTACK;
		}*/

		if ( cmd->cmd.nAttackStartHistoryIndex >= 0 ) {
			const auto sub_tick = cmd->cmd.inputHistoryField.pRep->tElements[ cmd->cmd.nAttackStartHistoryIndex ];

			//if ( !sub_tick->pViewCmd || !sub_tick->sv_interp0 || !sub_tick->sv_interp1 )
			//	return result;

			CLagRecord* record{ nullptr };

			for ( auto& e : ctx.m_mapPlayerEntries ) {
				auto& entry{ e.second };
				if ( entry.m_pPawn == localPawn )
					continue;

				if ( !entry.Animations.m_vecLagRecords.size( ) )
					continue;

				for ( auto& r : entry.Animations.m_vecLagRecords ) {
					if ( r.IsRecordValid( ) ) {
						record = &r;
						break;
					}
				}

				if ( record != nullptr )
					break;
			}

			if ( !record )
				return result;

			std::memcpy( ctx.DEBUGBacktrackBones, record->m_arrBones, sizeof( record->m_arrBones ) );

			cmd->cmd.pBase->nTickCount = record->m_nAddedTickCount;

			sub_tick->nPlayerTickCount = record->nRenderTickCount + 1;
			sub_tick->flPlayerTickFraction = record->flRenderTickFraction + 0.0012f;

			sub_tick->nRenderTickCount = record->nRenderTickCount;
			sub_tick->flRenderTickFraction = record->flRenderTickFraction;

			//sub_tick->cl_interp = sub_tick->sv_interp0 = sub_tick->sv_interp1 = nullptr;

			//PRINT_PTR( &( sub_tick->flRenderTickFraction ) );

			if ( sub_tick->cl_interp && sub_tick->sv_interp0 && sub_tick->sv_interp1 ) {
				sub_tick->cl_interp->nSrcTick = record->m_nAddedTickCount;
				sub_tick->cl_interp->nDstTick = record->m_nAddedTickCount + 1;
				sub_tick->cl_interp->flFraction = 0.f;

				sub_tick->sv_interp0->nSrcTick = record->m_nAddedTickCount - 1;
				sub_tick->sv_interp0->nDstTick = record->m_nAddedTickCount;
				sub_tick->sv_interp0->flFraction = 0.f;

				sub_tick->sv_interp1->nSrcTick = record->m_nAddedTickCount;
				sub_tick->sv_interp1->nDstTick = record->m_nAddedTickCount + 1;
				sub_tick->sv_interp1->flFraction = 0.f;
			}

			/*if ( cmd->cmd.pBase->subtickMovesField.pRep ) {
				//PRINT_PTR( cmd->cmd.pBase->subtickMovesField.pRep );
				cmd->cmd.pBase->subtickMovesField.nCurrentSize += 1;
				cmd->cmd.pBase->subtickMovesField.nTotalSize += 1;
				cmd->cmd.pBase->subtickMovesField.pRep->nAllocatedSize += 1;

				auto& element{ cmd->cmd.pBase->subtickMovesField.pRep->tElements[ cmd->cmd.pBase->subtickMovesField.pRep->nAllocatedSize - 1 ] };
				element = ( CSubtickMoveStep* )Interfaces::MemAlloc->Alloc( sizeof( CSubtickMoveStep ) );

				//PRINT_PTR( &cmd->cmd.pBase->subtickMovesField.pRep->tElements[ 0 ] );

				element->bPressed = true;
				element->nButton = IN_ATTACK;
				element->flWhen = record->flRenderTickFraction;
			}*/

			//ctx.m_flUpmove = sub_tick->cl_interp ? 1.f : 0.f;
		}
		
	}

	return result;
}

void __fastcall Hooks::hkUnknown01( void* rcx, int edx, char a3, unsigned char a4 ) {
	const auto og{ Unknown01.Original<decltype( &hkUnknown01 )>( ) };

	Vector d;
	Interfaces::Client->GetViewAngles( &d );

	og( rcx, edx, a3, a4 );

	Interfaces::Input->SetViewAngles( d );
}

void* __fastcall Hooks::hkLagcompensation( void* subTickData, void* inputHistoryFieldCurrent, char a3, __int64 a4, __int64 a5, __int64 a6 ) {
	const auto og{ Lagcompensation.Original<decltype( &hkLagcompensation )>( ) };

	auto std{ ( ( CSubTickData* ) subTickData ) };
	auto ihe{ ( ( CCSGOInputHistoryEntryPB* ) inputHistoryFieldCurrent ) };

	ctx.m_flRenderTickFraction = std->m_flRenderTickFraction;
	ctx.m_iRenderTick = std->m_iRenderTick;

	return og( subTickData, inputHistoryFieldCurrent, a3, a4, a5, a6 );
}