#ifndef CLIENT_GAME_INTERFACES_H
#define CLIENT_GAME_INTERFACES_H

#include "icamera.h"
#include "iinput.h"

// Class for managing providers for the client side of the game.
class CClientGameInterfaces
{

public:

	// Get the provider for the providers.
	static inline ICamera *GetCamera( void ) { return m_pCamera; }
	static IInput *GetInput( void ) { return m_pInput; }

	// Update the providers.
	static void SetCamera( ICamera *pCamera );
	static void SetInput( IInput *pInput );

private:

	static ICamera *m_pCamera;
	static IInput *m_pInput;

};

#endif // CLIENT_GAME_INTERFACES_H