#include "input.h"
#include "../core/config.h"

#define DODAKEYBINDCUH( bindname ) bindname.Resolve( );

void Input::CheckBinds( ) {
	DODAKEYBINDCUH( Configs::m_cConfig.m_kBugWalk );
	DODAKEYBINDCUH( Configs::m_cConfig.m_kThirdperson );
	DODAKEYBINDCUH( Configs::m_cConfig.m_kRageBotOverrideDamage );
}