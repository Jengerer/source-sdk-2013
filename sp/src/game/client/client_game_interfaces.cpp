#include "client_game_interfaces.h"

// Initialize static pointers.
ICamera *CClientGameInterfaces::m_pCamera = NULL;
IInput *CClientGameInterfaces::m_pInput = NULL;

// Get the camera interface.
ICamera *CClientGameInterfaces::GetCamera( void )
{
	return m_pCamera;
}

// Get the input interface.
IInput *CClientGameInterfaces::GetInput( void )
{
	return m_pInput;
}

// Set the camera interface.
void CClientGameInterfaces::SetCamera( ICamera *pCamera )
{
	m_pCamera = pCamera;
}

// Set the input interface.
void CClientGameInterfaces::SetInput( IInput *pInput )
{
	m_pInput = pInput;
}
