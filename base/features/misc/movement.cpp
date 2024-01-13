#include "movement.h"
#include "../../core/config.h"

void CMovement::Main( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	const auto oldButtons{ cmd->buttonStates.nButtonState1 };
	const auto oldButtons2{ cmd->buttonStates.nButtonState2 };
	const auto onGround{ ( local->m_fFlags( ) & FL_ONGROUND ) };
	static bool lastOnGround{ };
	if ( Configs::m_cConfig.m_bBunnyhop ) {
		if ( onGround )
			cmd->RemoveButton( IN_JUMP );
	}	

	if ( Configs::m_cConfig.m_bBugWalk ) {
		if ( onGround )
			cmd->buttonStates.nButtonState1 &= ~IN_JUMP;
	}

	lastOnGround = onGround;

	/*if ( cmd->buttonStates.nButtonState1 != oldButtons ) {
		const auto base{ cmd->cmd.pBase };
		if ( base ) {
			const auto rep{ base->subtickMovesField.pRep };
			if ( rep ) {
				for ( int i{ }; i < base->subtickMovesField.nCurrentSize; ++i ) {
					const auto entry{ rep->tElements[ i ] };
					if ( entry->nButton == IN_JUMP ) {
						printf( "%i -> %f\n", ( int ) entry->bPressed, entry->flWhen );
					}
				}
				/*base->subtickMovesField.nCurrentSize += 1;
				auto pInputEntry = rep->tElements[ base->subtickMovesField.nCurrentSize - 1 ];
				pInputEntry->bPressed = ( cmd->buttonStates.nButtonState1 & IN_JUMP );
				pInputEntry->flWhen = 0.01f;
				pInputEntry->nButton = IN_JUMP;*
			}
		}
	}*/
	
	if ( Configs::m_cConfig.m_bBugWalk ) {
		if ( local->m_fFlags( ) & FL_ONGROUND )
			cmd->AddButton( IN_JUMP );
		else
			cmd->RemoveButton( IN_JUMP );
	}
}