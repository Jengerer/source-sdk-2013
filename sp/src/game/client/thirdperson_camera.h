#ifndef THIRDPERSON_CAMERA_H
#define THIRDPERSON_CAMERA_H

#if defined( _WIN32 )
#pragma once
#endif

#include "c_baseplayer.h"

// Camera managing class for third person controls.
class CThirdPersonCamera
{

public:

	CThirdPersonCamera( void );
	~CThirdPersonCamera( void );

	// Set the bounds for the camera when tracing against objects.
	void SetCameraHull( const Vector& min, const Vector& max );

	// Update camera limits.
	void SetMinimumDistance( float minDistance );
	void SetMaximumDistance( float maxDistance );
	void SetMinimumYaw( float minYaw );
	void SetMaximumYaw( float maxYaw );
	void SetMinimumPitch( float minPitch );
	void SetMaximumPitch( float maxPitch );

	// Camera movement functions.
	void Zoom( float distanceDelta );
	void Turn( const QAngle &turnAngles );

	// Camera updating functions.
	void UpdateCamera( C_BasePlayer *pPlayer );

	// Camera position/angle polling.
	const Vector& GetPosition( void );
	const QAngle& GetAngles( void );

protected:

	// Distance updating function.
	void SetDistance( float distance );
	
private:

	// Camera hull bounds.
	Vector m_hullMin;
	Vector m_hullMax;

	// Camera limits.
	float m_minDistance;
	float m_maxDistance;
	float m_minPitch;
	float m_maxPitch;
	
	// Camera location.
	Vector m_position;

	// Camera distance and angles.
	float m_distance;
	QAngle m_angles;

};


#endif // THIRDPERSON_CAMERA_H
