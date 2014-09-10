//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//

#if !defined( INPUT_MANAGER_H )
#define INPUT_MANAGER_H
#ifdef _WIN32
#pragma once
#endif

#include "icamera.h"
#include "iinput.h"
#include "mathlib/vector.h"
#include "kbutton.h"
#include "ehandle.h"
#include "inputsystem/AnalogCode.h"
#include "mouse_manager.h"

typedef unsigned int CRC32_t;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CKeyboardKey
{
public:
	// Name for key
	char				name[ 32 ];
	// Pointer to the underlying structure
	kbutton_t			*pkey;
	// Next key in key list.
	CKeyboardKey		*next;
};

class ConVar;

/* Base input manager class. */
class CInputManager : public IInput
{

public:

	CInputManager( void );
	~CInputManager( void );

	// Initialize and clean up.
	virtual		void		Initialize( void );
	virtual		void		Shutdown( void );

	// Get a handle to the mouse input manager.
	virtual IMouse *GetMouse( void );

	// Player movement and command handling.
	virtual		int			GetButtonBits( int );
	virtual		void		CreateMove( int sequence_number, float input_sample_frametime, bool active );
	virtual		void		ExtraMouseSample( float frametime, bool active );
	virtual		bool		WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand );
	virtual		void		EncodeUserCmdToBuffer( bf_write& buf, int slot );
	virtual		void		DecodeUserCmdFromBuffer( bf_read& buf, int slot );
	virtual		CUserCmd	*GetUserCmd( int sequence_number );
	virtual		void		MakeWeaponSelection( C_BaseCombatWeapon *weapon );

	// Keyboard key handling.
	virtual		float		KeyState( kbutton_t *key );
	virtual		int			KeyEvent( int down, ButtonCode_t keynum, const char *pszCurrentBinding );
	virtual		kbutton_t	*FindKey( const char *name );

	// Controller and joystick handling.
	virtual		void		ControllerCommands( void );
	virtual		void		Joystick_Advanced( void );
	virtual		void		Joystick_SetSampleTime(float frametime);
	virtual		void		IN_SetSampleTime( float frametime );
	virtual		float		Joystick_GetForward( void );
	virtual		float		Joystick_GetSide( void );
	virtual		float		Joystick_GetPitch( void );
	virtual		float		Joystick_GetYaw( void );

	// Mouse handling.
	virtual		float		GetLookSpring( void );

//	virtual		bool		IsNoClipping( void );
	virtual		float		GetLastForwardMove( void );
	virtual		void		ClearInputButton( int bits );

#if defined( HL2_CLIENT_DLL )
	// IK back channel info
	virtual		void		AddIKGroundContactInfo( int entindex, float minheight, float maxheight );
#endif
	virtual		void		LevelInit( void );

	virtual	bool		EnableJoystickMode();

private:

	// Initializing input components.
	void		InitializeKeyboard( void );
	void		ShutdownKeyboard( void );

	// Add a named key to the list queryable by the engine
	void		AddKeyButton( const char *name, kbutton_t *pkb );

	// Mouse input helpers.
	void		AdjustPitch( float speed, QAngle& viewangles );
	virtual void AdjustYaw( float speed, QAngle& viewangles );
	void		ScaleMovements( CUserCmd *cmd );
	void		ComputeForwardMove( CUserCmd *cmd );
	void		ComputeUpwardMove( CUserCmd *cmd );
	void		ComputeSideMove( CUserCmd *cmd );
	void		AdjustAngles ( float frametime );
	void		ClampAngles( QAngle& viewangles );

	float		DetermineKeySpeed( float frametime );

	// Joystick movement input helpers
	void		ControllerMove ( float frametime, CUserCmd *cmd );
	void		JoyStickMove ( float frametime, CUserCmd *cmd );
	float		ScaleAxisValue( const float axisValue, const float axisThreshold );
	virtual float JoyStickAdjustYaw( float flSpeed ) { return flSpeed; }

	void		ValidateUserCmd( CUserCmd *usercmd, int sequence_number );

// Private Data
private:

	// Camera managing class to pass mouse events to.
	ICamera *m_pCamera;

	// Mouse manager class.
	CMouseManager m_mouseManager;

	typedef struct
	{
		unsigned int AxisFlags;
		unsigned int AxisMap;
		unsigned int ControlMap;
	} joy_axis_t;

	void		DescribeJoystickAxis( char const *axis, joy_axis_t *mapping );
	char const	*DescribeAxis( int index );

	enum
	{
		CAM_COMMAND_NONE = 0,
		CAM_COMMAND_TOTHIRDPERSON = 1,
		CAM_COMMAND_TOFIRSTPERSON = 2
	};

	// Has the joystick advanced initialization been run?
	bool		m_fJoystickAdvancedInit;
	// Used to support hotplugging by reinitializing the advanced joystick system when we toggle between some/none joysticks.
	bool		m_fHadJoysticks;

	float		m_flRemainingJoystickSampleTime;
	float		m_flKeyboardSampleTime;


	// Joystick Axis data
	joy_axis_t m_rgAxes[ MAX_JOYSTICK_AXES ];
	// List of queryable keys
	CKeyboardKey *m_pKeys;

	// orthographic camera settings
	QAngle		m_angPreviousViewAngles;
	float		m_flLastForwardMove;

	float m_flPreviousJoystickForward;
	float m_flPreviousJoystickSide;
	float m_flPreviousJoystickPitch;
	float m_flPreviousJoystickYaw;

	class CVerifiedUserCmd
	{
	public:
		CUserCmd	m_cmd;
		CRC32_t		m_crc;
	};
				
	CUserCmd	*m_pCommands;
	CVerifiedUserCmd *m_pVerifiedCommands;

	// Set until polled by CreateMove and cleared
	CHandle< C_BaseCombatWeapon > m_hSelectedWeapon;

#if defined( HL2_CLIENT_DLL )
	CUtlVector< CEntityGroundContact > m_EntityGroundContact;
#endif
};

extern kbutton_t in_strafe;
extern kbutton_t in_speed;
extern kbutton_t in_jlook;
extern kbutton_t in_graph;  
extern kbutton_t in_moveleft;
extern kbutton_t in_moveright;
extern kbutton_t in_forward;
extern kbutton_t in_back;
extern kbutton_t in_joyspeed;

extern class ConVar in_joystick;
extern class ConVar joy_autosprint;

#endif // INPUT_MANAGER_H
	
