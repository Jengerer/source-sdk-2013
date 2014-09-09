#ifndef BOKO_GAME_INTERFACES_H
#define BOKO_GAME_INTERFACES_H

#include "client_game_interfaces.h"
#include "camera_manager.h"

// Class that defines the implementing game interfaces.
class CBokoGameInterfaces : public CClientGameInterfaces
{

public:

	// Initialize the implementing objects.
	static void Initialize( void );

private:

	// The implementing classes.

};

#endif // BOKO_GAME_INTERFACES_H