//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#if !defined( IINPUT_H )
#define IINPUT_H
#ifdef _WIN32
#pragma once
#endif

#include "imouse.h"

class bf_write;
class bf_read;
class CUserCmd;
class C_BaseCombatWeapon;
struct kbutton_t;

abstract_class IInput
{
public:
	// Initialization/shutdown of the subsystem
	virtual	void		Initialize( void ) = 0;
	virtual void		Shutdown( void ) = 0;

	// Get a handle to the mouse manager.
	virtual IMouse *GetMouse( void ) = 0;

	// Latching button states
	virtual int			GetButtonBits( int ) = 0;
	// Create movement command
	virtual void		CreateMove ( int sequence_number, float input_sample_frametime, bool active ) = 0;
	virtual void		ExtraMouseSample( float frametime, bool active ) = 0;
	virtual bool		WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand ) = 0;
	virtual void		EncodeUserCmdToBuffer( bf_write& buf, int slot ) = 0;
	virtual void		DecodeUserCmdFromBuffer( bf_read& buf, int slot ) = 0;

	virtual CUserCmd	*GetUserCmd( int sequence_number ) = 0;

	virtual void		MakeWeaponSelection( C_BaseCombatWeapon *weapon ) = 0;

	// Retrieve key state
	virtual float		KeyState ( kbutton_t *key ) = 0;
	// Issue key event
	virtual int			KeyEvent( int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding ) = 0;
	// Look for key
	virtual kbutton_t	*FindKey( const char *name ) = 0;

	// Issue commands from controllers
	virtual void		ControllerCommands( void ) = 0;
	// Extra initialization for some joysticks
	virtual void		Joystick_Advanced( void ) = 0;
	virtual void		Joystick_SetSampleTime( float frametime ) = 0;
	virtual void		IN_SetSampleTime( float frametime ) = 0;

	// Retrieve lookspring setting
	virtual float		GetLookSpring( void ) = 0;

	virtual	float		GetLastForwardMove( void ) = 0;
	virtual	float		Joystick_GetForward( void ) = 0;
	virtual	float		Joystick_GetSide( void ) = 0;
	virtual	float		Joystick_GetPitch( void ) = 0;
	virtual	float		Joystick_GetYaw( void ) = 0;

#if defined( HL2_CLIENT_DLL )
	// IK back channel info
	virtual void		AddIKGroundContactInfo( int entindex, float minheight, float maxheight ) = 0;
#endif

	virtual void		LevelInit( void ) = 0;

	// Causes an input to have to be re-pressed to become active
	virtual void		ClearInputButton( int bits ) = 0;
	
	virtual	bool		EnableJoystickMode() = 0;
};

extern void KeyDown( kbutton_t *b, const char *c );
extern void KeyUp( kbutton_t *b, const char *c );

#endif // IINPUT_H