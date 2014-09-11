#if defined( WIN32 ) && !defined( _X360 )
#define _WIN32_WINNT 0x0502
#include <windows.h>
#endif


#include "cbase.h"
#include "mouse_manager.h"
#include "cdll_client_int.h"
#include "vgui/ISurface.h"
#include "vgui/IInput.h"
#include "vgui/Cursor.h"
#include "vgui_controls/Controls.h"
#include "inputsystem/iinputsystem.h"
#include "tier0/icommandline.h"

// Mouse enabled variable.
ConVar cl_mouseenable( "cl_mouseenable", "1" );

// Mouse input and filtering variables.
ConVar sensitivity( "sensitivity", "3", FCVAR_ARCHIVE, "Mouse sensitivity.", true, 0.0001f, true, 10000000 );
static ConVar m_rawinput( "m_rawinput", "0", FCVAR_ARCHIVE, "Use Raw Input for mouse input." );
static ConVar m_filter( "m_filter", "0", FCVAR_ARCHIVE, "Mouse filtering (set this to 1 to average the mouse over 2 frames)." );

// Mouse acceleration variables.
static ConVar m_mousespeed( "m_mousespeed", "1", FCVAR_ARCHIVE, "Windows mouse acceleration (0 to disable, 1 to enable [Windows 2000: enable initial threshold], 2 to enable secondary threshold [Windows 2000 only]).", true, 0, true, 2 );
static ConVar m_mouseaccel1( "m_mouseaccel1", "0", FCVAR_ARCHIVE, "Windows mouse acceleration initial threshold (2x movement).", true, 0, false, 0.0f );
static ConVar m_mouseaccel2( "m_mouseaccel2", "0", FCVAR_ARCHIVE, "Windows mouse acceleration secondary threshold (4x movement).", true, 0, false, 0.0f );

CMouseManager::CMouseManager( void )
	: m_bInitialized( false ),
	m_bActive( false ),
	m_bHasSavedParameters( false ),
	m_fPreviousSampleX( 0.0f ),
	m_fPreviousSampleY( 0.0f ),
	m_iAccumulatedX( 0 ),
	m_iAccumulatedY( 0 )
{
	// Set to use console variables for mouse acceleration settings by default.
	for (int i = 0; i < NUM_MOUSE_PARAMS; ++i) {
		m_bUseConsoleVariable[i] = true;
	}
}

CMouseManager::~CMouseManager( void )
{
}

// Mouse initialization.
void CMouseManager::Initialize( void )
{
	// Don't re-initialize.
	if (m_bInitialized) {
		return;
	}

	// Check if mouse is disabled.
	if (CommandLine()->FindParm( "-nomouse" )) {
		return;
	}

	m_bInitialized = true;
	SaveAccelerationSettings();
	CheckAccelerationOverride();
}

// Activate the mouse.
void CMouseManager::Activate( void )
{
	// Check if redundant.
	if (m_bActive) {
		return;
	}

	// Need to be initialized first.
	if (!m_bInitialized) {
		return;
	}

	// Move to center.
	ResetToWindowCenter();
	ResetAccumulatedMovement();
	m_bActive = true;
}

// Deactivate the mouse.
void CMouseManager::Deactivate( void )
{
	// Check that we're not already inactive.
	if (!m_bActive) {
		return;
	}

	// Mouse must be initialized.
	if (!m_bInitialized) {
		return;
	}

	// Restore acceleration settings if needed.
	RestoreAccelerationSettings();

	// Deactivate mouse and reset.
	m_bActive = false;
	vgui::surface()->SetCursor( vgui::dc_arrow );
	ResetAccumulatedMovement();
}

// Return whether the mouse is active.
bool CMouseManager::IsActive( void ) const
{
	return m_bActive;
}

// Update the mouse and return scaled movement.
void CMouseManager::GetMouseMovement( float *deltaX, float *deltaY )
{
	// Update mouse acceleration settings if they changed.
	ApplyAccelerationSettings();

	// TODO: make sure we disabled pitch drift in BokoInputManager.
	float moveX, moveY;
	if (!vgui::surface()->IsCursorVisible()) {
		// Sample movement (getting accumulated movement already resets it).
		AccumulateMovement();
		GetAccumulatedMovement( &moveX, &moveY );

		// Filter and apply sensitivity.
		FilterMouseDelta( &moveX, &moveY );
		ScaleMouseDelta( &moveX, &moveY );
	}
	else {
		moveX = 0.0f;
		moveY = 0.0f;
	}

	// Output result.
	*deltaX = moveX;
	*deltaY = moveY;
}

// Get the current cursor position.
void CMouseManager::GetCursorPosition( int *pX, int *pY )
{
	vgui::input()->GetCursorPos( *pX, *pY );
}

// Set the current cursor position.
void CMouseManager::SetCursorPosition( int x, int y )
{
	vgui::input()->SetCursorPos( x, y );
}

// Reset mouse to window center position.
void CMouseManager::ResetToWindowCenter( void )
{
	int centerX, centerY;
	GetWindowCenter( &centerX, &centerY );
	SetCursorPosition( centerX, centerY );
}

// Calculate current accumulation for mouse and reset to center if tracking.
void CMouseManager::AccumulateMovement( void )
{
	// Check if mouse is disabled.
	if (!cl_mouseenable.GetBool()) {
		return;
	}

	// If raw input is enabled, drive accumulates for us.
	if (m_rawinput.GetBool()) {
		return;
	}

	int currentX, currentY;
	GetCursorPosition( &currentX, &currentY );

	// Only accumulate if cursor is locked.
	if (vgui::surface()->IsCursorLocked()) {
#if defined( PLATFORM_WINDOWS )
		// Get difference from center screen.
		int centerX, centerY;
		GetWindowCenter( &centerX, &centerY );
		m_iAccumulatedX += (currentX - centerX);
		m_iAccumulatedY += (currentY - centerY);
#elif defined( USE_SDL )
		// Get from engine.
		int deltaX, deltaY;
		engine->GetMouseDelta( deltaX, deltaY );
		m_iAccumulatedX += deltaX;
		m_iAccumulatedY += deltaY;
#endif

		ResetToWindowCenter();
	}
	else {
		// Just clamp to borders otherwise.
		int screenWidth, screenHeight;
		GetWindowSize( &screenWidth, &screenHeight );
		currentX = Clamp( currentX, 0, screenWidth - 1 );
		currentY = Clamp( currentY, 0, screenHeight - 1 );
		SetCursorPosition( currentX, currentY );
	}
}

// Get current accumulated mouse movement and reset it.
void CMouseManager::GetAccumulatedMovement( float *pX, float *pY )
{
	// If we're getting raw mouse input, get from the input system.
	bool returnedRaw = false;
	if (m_rawinput.GetBool()) {
		int rawX, rawY;
		if (inputsystem->GetRawMouseAccumulators( rawX, rawY )) {
			*pX = (float)rawX;
			*pY = (float)rawY;
			returnedRaw = true;
		}
	}

	// Use internally accumulated otherwise.
	if (!returnedRaw) {
		*pX = (float)m_iAccumulatedX;
		*pY = (float)m_iAccumulatedY;
		m_iAccumulatedX = 0;
		m_iAccumulatedY = 0;
	}
}

// Reset accumulated movement (for when mouse is activated).
void CMouseManager::ResetAccumulatedMovement( void )
{
	// Reset input accumulation.
	int rawX, rawY;
	inputsystem->GetRawMouseAccumulators( rawX, rawY );

	// Reset internally accumulated.
	m_iAccumulatedX = 0;
	m_iAccumulatedY = 0;
}

// Filter the mouse delta using the previous two samples if enabled.
void CMouseManager::FilterMouseDelta( float *pX, float *pY )
{
	// Get current sample.
	float deltaX = *pX;
	float deltaY = *pY;

	// Check if filtering is on.
	if (m_filter.GetBool()) {
		// Average with last sample.
		*pX = (deltaX + m_fPreviousSampleX) * 0.5f;
		*pY = (deltaY + m_fPreviousSampleY) * 0.5f;
	}

	// Update new sample.
	m_fPreviousSampleX = deltaX;
	m_fPreviousSampleY = deltaY;
}

// Scale the mouse delta by the sensitivity.
void CMouseManager::ScaleMouseDelta( float *pX, float *pY )
{
	// Get current sample.
	float deltaX = *pX;
	float deltaY = *pY;

	// Scale by sensitivity value.
	float mouseSensitivity = (gHUD.GetSensitivity() != 0) ? gHUD.GetSensitivity() : sensitivity.GetFloat();
	*pX = deltaX * mouseSensitivity;
	*pY = deltaY * mouseSensitivity;
}

// Set up and apply mouse acceleration settings.
void CMouseManager::CheckAccelerationOverride( void )
{
#if defined (WIN32)
	// Check if user is changing acceleration settings.
	if (CommandLine()->FindParm( "-noforcedmparms" )) {
		// Check if user wants to keep current thresholds.
		if (CommandLine()->FindParm( "-noforcemaccel" )) {
			m_newParameters[MOUSE_ACCEL_THRESHOLD_1] = m_oldParameters[MOUSE_ACCEL_THRESHOLD_1];
			m_newParameters[MOUSE_ACCEL_THRESHOLD_2] = m_oldParameters[MOUSE_ACCEL_THRESHOLD_2];
			m_bUseConsoleVariable[MOUSE_ACCEL_THRESHOLD_1] = false;
			m_bUseConsoleVariable[MOUSE_ACCEL_THRESHOLD_2] = false;
		}

		// Check if user wants to keep current speed.
		if (CommandLine()->FindParm( "-noforcemspd" )) {
			m_newParameters[MOUSE_SPEED_FACTOR] = m_oldParameters[MOUSE_SPEED_FACTOR];
			m_bUseConsoleVariable[MOUSE_SPEED_FACTOR] = false;
		}
	}
#endif
}

// Apply acceleration settings if they have changed.
void CMouseManager::ApplyAccelerationSettings( void )
{
#if defined( WIN32 )
	// Don't apply settings if we couldn't save the old ones.
	if (!m_bHasSavedParameters) {
		return;
	}

	// Get console variable values.
	int consoleSettings[NUM_MOUSE_PARAMS];
	consoleSettings[MOUSE_ACCEL_THRESHOLD_1] = m_mouseaccel1.GetInt();
	consoleSettings[MOUSE_ACCEL_THRESHOLD_2] = m_mouseaccel2.GetInt();
	consoleSettings[MOUSE_SPEED_FACTOR] = m_mousespeed.GetInt();

	// Go through the settings and mark if there's a difference.
	bool hasChanged = false;
	for (int i = 0; i < NUM_MOUSE_PARAMS; ++i) {
		// Are we supposed to use this value?
		if (!m_bUseConsoleVariable[i]) {
			continue;
		}

		// Is this different from what we have applied?
		if (consoleSettings[i] != m_newParameters[i]) {
			hasChanged = true;
			m_newParameters[i] = consoleSettings[i];
		}
	}

	// Apply new settings.
	if (hasChanged) {
		SystemParametersInfo( SPI_SETMOUSE, 0, m_newParameters, 0 );
	}
#endif
}

// Save acceleration settings.
void CMouseManager::SaveAccelerationSettings( void )
{
#if defined( WIN32 )
	m_bHasSavedParameters = SystemParametersInfo( SPI_GETMOUSE, 0, m_oldParameters, 0 );
#endif
}

// Restore acceleration settings.
void CMouseManager::RestoreAccelerationSettings( void )
{
#if defined( WIN32 )
	if (m_bHasSavedParameters) {
		SystemParametersInfo( SPI_SETMOUSE, 0, m_oldParameters, 0 );
	}
#endif
}

// Helper for getting window size.
void CMouseManager::GetWindowSize( int *pWidth, int *pHeight )
{
	engine->GetScreenSize( *pWidth, *pHeight );
}

// Helper for getting center position of window.
void CMouseManager::GetWindowCenter( int *pX, int *pY )
{
	int screenWidth, screenHeight;
	GetWindowSize( &screenWidth, &screenHeight );
	*pX = screenWidth >> 1;
	*pY = screenHeight >> 1;
}
