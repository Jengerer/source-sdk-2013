#ifndef ICAMERA_H
#define ICAMERA_H

#include "c_baseentity.h"

// Interface for a game's camera manager.
class ICamera
{

public:

	// Get the vector and angle of the camera.
	virtual	void CalculateCameraView( Vector &position, QAngle &angles ) = 0;

	// Handle mouse movement event.
	virtual void HandleMouse( float deltaX, float deltaY ) = 0;

	// Prepare camera for this frame.
	virtual void Think( void ) = 0;

	// Get camera state.
	virtual bool IsFirstPerson( void ) = 0;
	virtual bool IsThirdPerson( void ) = 0;

	// Switch camera modes.
	virtual void ToThirdPerson( void ) = 0;
	virtual void ToFirstPerson( void ) = 0;

};

#endif // ICAMERA_H