#include "antiaim.h"

// TODO: detected! look at nConsumedServerAngleChanges (pretty sure it is a checksum but havent reversed it)

void CAntiAim::Main( C_CSPlayerPawn* local, CUserCmd* cmd ) {
	cmd->cmd.pBase->pViewangles->angValue.x = 89.f;

	/*auto& SubTicks = cmd->cmd.inputHistoryField;
	for ( int i = 0; i < SubTicks.nCurrentSize; i++ )
	{
		auto* Tick = SubTicks.pRep->tElements[ i ];
		if ( Tick && Tick->pViewCmd )
		{
			Tick->pViewCmd->angValue.x = 89.f;
		}

		if ( Tick && Tick->pTargetViewCmd )
		{
			Tick->pTargetViewCmd->angValue.x = 89.f;
		}
	}*/
}