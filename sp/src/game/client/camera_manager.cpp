#include "cbase.h"
#include "camera_manager.h"

// Set up camera manager.
CCameraManager::CCameraManager( void )
{
}

// Camera manager destructor.
CCameraManager::~CCameraManager( void )
{
}

// Pass mouse event to active camera controller.
void CCameraManager::HandleMouse( float yawDelta, float pitchDelta )
{
	QAngle turnAngles;
	turnAngles[YAW] = yawDelta;
	turnAngles[PITCH] = pitchDelta;
	turnAngles[ROLL] = 0.0f;
	m_thirdPerson.Turn( turnAngles );
}

// Update the camera for this frame.
void CCameraManager::CameraThink( void )
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer != NULL) {
		m_thirdPerson.UpdateCamera( pPlayer );
	}
}

// Get current camera position.
Vector CCameraManager::GetPosition( void )
{
	return m_thirdPerson.GetPosition();
}

// Get current camera angles.
QAngle CCameraManager::GetAngles( void )
{
	return m_thirdPerson.GetAngles();
}
