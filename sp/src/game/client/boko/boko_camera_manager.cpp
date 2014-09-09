#include "cbase.h"
#include "camera_manager.h"

// Set up camera manager.
CBokoCameraManager::CCameraManager( void )
{
}

// Camera manager destructor.
CBokoCameraManager::~CCameraManager( void )
{
}

// Pass mouse event to active camera controller.
void CBokoCameraManager::HandleMouse( float yawDelta, float pitchDelta )
{
	QAngle turnAngles;
	turnAngles[YAW] = yawDelta;
	turnAngles[PITCH] = pitchDelta;
	turnAngles[ROLL] = 0.0f;
	m_thirdPerson.Turn( turnAngles );
}

// Update the camera for this frame.
void CBokoCameraManager::CameraThink( void )
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer != NULL) {
		m_thirdPerson.UpdateCamera( pPlayer );
	}
}

// Get current camera position.
Vector CBokoCameraManager::GetPosition( void )
{
	return m_thirdPerson.GetPosition();
}

// Get current camera angles.
QAngle CBokoCameraManager::GetAngles( void )
{
	return m_thirdPerson.GetAngles();
}
