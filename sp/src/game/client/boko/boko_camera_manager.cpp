#include "cbase.h"
#include "boko_camera_manager.h"
#include "tier1/convar_serverbounded.h"

// Camera turning speeds.
static ConVar m_pitch( "m_pitch", "0.22", FCVAR_ARCHIVE, "Mouse pitch factor." );
static ConVar m_yaw( "m_yaw", "0.022", FCVAR_ARCHIVE, "Mouse yaw factor." );

// Set up camera manager.
CBokoCameraManager::CBokoCameraManager( void )
{
}

// Camera manager destructor.
CBokoCameraManager::~CBokoCameraManager( void )
{
}

// Get the position and angles of the camera for htis frame.
void CBokoCameraManager::CalculateCameraView( Vector &position, QAngle &angles )
{
	position = GetPosition();
	angles = GetAngles();
}

// Pass mouse event to active camera controller.
void CBokoCameraManager::HandleMouse( float x, float y )
{
	// Apply turning speeds.
	float yawTurn = -x * m_yaw.GetFloat();
	float pitchTurn = y * m_pitch.GetFloat();

	// Apply turn.
	QAngle turnAngles;
	turnAngles[YAW] = yawTurn;
	turnAngles[PITCH] = pitchTurn;
	turnAngles[ROLL] = 0.0f;
	m_thirdPerson.Turn( turnAngles );

	// Update engine angles.
	QAngle viewAngles = m_thirdPerson.GetAngles();
	engine->SetViewAngles( viewAngles );
}

// Update the camera for this frame.
void CBokoCameraManager::Think( void )
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer != NULL) {
		m_thirdPerson.UpdateCamera( pPlayer );
	}
}

// Return whether we're in first person.
bool CBokoCameraManager::IsFirstPerson( void )
{
	return false;
}

// Return whether we're in third person.
bool CBokoCameraManager::IsThirdPerson( void )
{
	return true;
}

// Switch to third person if possible.
void CBokoCameraManager::ToThirdPerson( void )
{
}

// Switch to first person if possible.
void CBokoCameraManager::ToFirstPerson( void )
{
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
