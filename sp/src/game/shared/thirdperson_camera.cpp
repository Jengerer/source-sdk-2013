#include "cbase.h"
#include "thirdperson_camera.h"
#include "gamerules.h"
#include "tier0/memdbgon.h"

// Default camera hull size.
static const Vector DefaultThirdPersonHullMin( -20.0f, -20.0f, -20.0f );
static const Vector DefaultThirdPersonHullMax( 20.0f, 20.0f, 20.0f );

// Default camera limits.
static const float DefaultThirdPersonMinimumDistance = 100.0f;
static const float DefaultThirdPersonMaximumDistance = 500.0f;
static const float DefaultThirdPersonMinimumPitch = -75.0f;
static const float DefaultThirdPersonMaximumPitch = 75.0f;

// Default camera angles.
static const float DefaultThirdPersonYaw = 0.0f;
static const float DefaultThirdPersonPitch = 0.0f;
static const float DefaultThirdPersonRoll = 0.0f;

// Third person camera constructor.
// Set default parameters.
CThirdPersonCamera::CThirdPersonCamera( void )
{
	// Set default hull.
	SetCameraHull( DefaultThirdPersonHullMin, DefaultThirdPersonHullMax );

	// Set distance limits.
	SetMinimumDistance( DefaultThirdPersonMinimumDistance );
	SetMaximumDistance( DefaultThirdPersonMaximumDistance );

	// Set pitch limits.
	SetMinimumPitch( DefaultThirdPersonMinimumPitch );
	SetMaximumPitch( DefaultThirdPersonMaximumPitch );

	// Set default angles.
	m_angles[YAW] = DefaultThirdPersonYaw;
	m_angles[PITCH] = DefaultThirdPersonPitch;
	m_angles[ROLL] = DefaultThirdPersonRoll;

	// Set default distance.
	SetDistance( DefaultThirdPersonMaximumDistance );
}

// Third person camera destructor.
CThirdPersonCamera::~CThirdPersonCamera( void )
{
}

// Set the camera's trace hull.
void CThirdPersonCamera::SetCameraHull( const Vector& min, const Vector& max )
{
	VectorCopy( min, m_hullMin );
	VectorCopy( max, m_hullMax );
}

// Set minimum camera distance.
void CThirdPersonCamera::SetMinimumDistance( float minDistance )
{
	m_minDistance = minDistance;
}

// Set maximum camera distance.
void CThirdPersonCamera::SetMaximumDistance( float maxDistance )
{
	m_maxDistance = maxDistance;
}

// Set minimum camera pitch.
void CThirdPersonCamera::SetMinimumPitch( float minPitch )
{
	m_minPitch = minPitch;
}

// Set maximum camera pitch.
void CThirdPersonCamera::SetMaximumPitch( float maxPitch )
{
	m_maxPitch = maxPitch;
}

// Change the camera's current zoom level.
void CThirdPersonCamera::Zoom( float distanceDelta )
{
	// Add and clamp.
	SetDistance( m_distance + distanceDelta);
}

// Turn the camera.
void CThirdPersonCamera::Turn( const QAngle &turnAngles )
{
	// Add and clamp angles.
	m_angles += turnAngles;
	m_angles[PITCH] = Clamp( m_angles[PITCH], m_minPitch, m_maxPitch );
}

// Update camera position using current angles/distance.
void CThirdPersonCamera::UpdateCamera( CBasePlayer *pPlayer )
{
	// Get player position.
	const Vector& playerOrigin = pPlayer->GetAbsOrigin();
	Vector viewOrigin = playerOrigin + pPlayer->GetViewOffset();

	// Get camera's forward vector and move back by distance.
	Vector forward;
	Vector endPosition;
	AngleVectors( m_angles, &forward );
	VectorMA( playerOrigin, -m_distance, forward, endPosition );

	// Trace for collision.
	trace_t trace;
	CTraceFilterSimple traceFilter( pPlayer, COLLISION_GROUP_NONE );
	UTIL_TraceHull( viewOrigin, endPosition, m_hullMin, m_hullMax, MASK_SOLID & ~CONTENTS_MONSTER, &traceFilter, &trace );
	m_position = trace.endpos;
}

// Get camera position.
const Vector& CThirdPersonCamera::GetPosition( void )
{
	return m_position;
}

// Get camera angles.
const QAngle& CThirdPersonCamera::GetAngles( void )
{
	return m_angles;
}

// Update camera distance.
void CThirdPersonCamera::SetDistance( float distance )
{
	m_distance = Clamp( distance, m_minDistance, m_maxDistance );
}
