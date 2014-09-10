//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Mouse input routines
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//===========================================================================//
#if defined( WIN32 ) && !defined( _X360 )
#define _WIN32_WINNT 0x0502
#include <windows.h>
#endif
#include "cbase.h"
#include "hud.h"
#include "cdll_int.h"
#include "kbutton.h"
#include "basehandle.h"
#include "usercmd.h"
#include "input.h"
#include "iviewrender.h"
#include "iclientmode.h"
#include "tier0/icommandline.h"
#include "vgui/ISurface.h"
#include "vgui_controls/Controls.h"
#include "vgui/Cursor.h"
#include "cdll_client_int.h"
#include "cdll_util.h"
#include "tier1/convar_serverbounded.h"
#include "inputsystem/iinputsystem.h"

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// up / down
#define	PITCH	0
// left / right
#define	YAW		1

#ifdef PORTAL
	bool g_bUpsideDown = false; // Set when the player is upside down in Portal to invert the mouse.
#endif //#ifdef PORTAL

extern ConVar lookstrafe;
extern ConVar cl_pitchdown;
extern ConVar cl_pitchup;
extern const ConVar *sv_cheats;

class ConVar_m_pitch : public ConVar_ServerBounded
{
public:
	ConVar_m_pitch() : 
		ConVar_ServerBounded( "m_pitch","0.022", FCVAR_ARCHIVE, "Mouse pitch factor." )
	{
	}
	
	virtual float GetFloat() const
	{
		if ( !sv_cheats )
			sv_cheats = cvar->FindVar( "sv_cheats" );

		// If sv_cheats is on then it can be anything.
		float flBaseValue = GetBaseFloatValue();
		if ( !sv_cheats || sv_cheats->GetBool() )
			return flBaseValue;

		// If sv_cheats is off than it can only be 0.022 or -0.022 (if they've reversed the mouse in the options).		
		if ( flBaseValue > 0 )
			return 0.022f;
		else
			return -0.022f;
	}
} cvar_m_pitch;
ConVar_ServerBounded *m_pitch = &cvar_m_pitch;

extern ConVar cam_idealyaw;
extern ConVar cam_idealpitch;
extern ConVar thirdperson_platformer;

static ConVar m_side( "m_side","0.8", FCVAR_ARCHIVE, "Mouse side factor." );
static ConVar m_yaw( "m_yaw","0.022", FCVAR_ARCHIVE, "Mouse yaw factor." );
static ConVar m_forward( "m_forward","1", FCVAR_ARCHIVE, "Mouse forward factor." );



#if DEBUG
ConVar cl_mouselook( "cl_mouselook", "1", FCVAR_ARCHIVE, "Set to 1 to use mouse for look, 0 for keyboard look." );
#else
ConVar cl_mouselook( "cl_mouselook", "1", FCVAR_ARCHIVE | FCVAR_NOT_CONNECTED, "Set to 1 to use mouse for look, 0 for keyboard look. Cannot be set while connected to a server." );
#endif
