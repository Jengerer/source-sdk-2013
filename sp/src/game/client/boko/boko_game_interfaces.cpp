#include "cbase.h"
#include "boko_game_interfaces.h"

CBokoCameraManager CBokoGameInterfaces::m_camera;
CBokoInputManager CBokoGameInterfaces::m_input;

// Initialize the implementing interface classes.
void CBokoGameInterfaces::Initialize( void )
{
	SetInput( &m_input );
	SetCamera( &m_camera );
}