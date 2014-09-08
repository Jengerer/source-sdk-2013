#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#if defined(WIN32)
#pragma once
#endif

#include "thirdperson_camera.h"

// Class that handles multiple camera controllers.
class CCameraManager
{

public:

	CCameraManager( void );
	~CCameraManager( void );

	// Camera switching functions.
	void SwitchToFirstPerson( void );
	void SwitchToThirdPerson( void );

	// Handle mouse movement for camera.
	void HandleMouse( float yawDelta, float pitchDelta );

	// Camera update function.
	void CameraThink( void );

	// Camera angle and position getters.
	Vector GetPosition( void );
	QAngle GetAngles( void );

private:

	// Camera controllers to manage.
	CThirdPersonCamera m_thirdPerson;

};

#endif // CAMERA_MANAGER_H