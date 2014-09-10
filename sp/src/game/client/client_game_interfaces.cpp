#include "cbase.h"
#include "client_game_interfaces.h"

// Initialize static pointers.
ICamera *CClientGameInterfaces::m_pCamera = NULL;
IInput *CClientGameInterfaces::m_pInput = NULL;

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
