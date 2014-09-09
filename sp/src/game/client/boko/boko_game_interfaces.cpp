#include "boko_game_interfaces.h"

/* Initialize the game's interfaces. */
void CBokoGameInterfaces::Initialize( void )
{
	SetCamera( m_camera );
	SetInput( m_input );
}

