#ifndef BOKO_CAMERA_MANAGER_H
#define BOKO_CAMERA_MANAGER_H

#if defined(WIN32)
#pragma once
#endif

#include "icamera.h"
#include "thirdperson_camera.h"

// Class that handles multiple camera controllers.
class CBokoCameraManager : public ICamera
{

public:

	CBokoCameraManager( void );
	~CBokoCameraManager( void );

	// Camera switching functions.
	void SwitchToFirstPerson( void );
	void SwitchToThirdPerson( void );

	// Handle mouse movement for camera.
	void HandleMouse( float deltaX, float deltaY );

	// Camera update function.
	virtual void Think( void );

	// Get camera state.
	virtual bool IsFirstPerson( void );
	virtual bool IsThirdPerson( void );

	// Switch camera state.
	virtual void ToThirdPerson( void );
	virtual void ToFirstPerson( void );

	// Camera angle and position getters.
	Vector GetPosition( void );
	QAngle GetAngles( void );

private:

	// Camera controllers to manage.
	CThirdPersonCamera m_thirdPerson;

};

#endif // BOKO_CAMERA_MANAGER_H
