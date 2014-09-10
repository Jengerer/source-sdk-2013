#ifndef CLIENT_GAME_INTERFACES_H
#define CLIENT_GAME_INTERFACES_H

#include "icamera.h"
#include "iinput.h"

// Class for managing providers for the client side of the game.
class CClientGameInterfaces
{

public:

	// Get the provider for the providers.
	inline static ICamera *GetCamera( void );
	inline static IInput *GetInput( void );

	// Update the providers.
	static void SetCamera( ICamera *pCamera );
	static void SetInput( IInput *pInput );

private:

	static ICamera *m_pCamera;
	static IInput *m_pInput;

};

// Get the camera interface.
inline ICamera *CClientGameInterfaces::GetCamera( void )
{
	return m_pCamera;
}

// Get the input interface.
inline IInput *CClientGameInterfaces::GetInput( void )
{
	return m_pInput;
}

#endif // CLIENT_GAME_INTERFACES_H
