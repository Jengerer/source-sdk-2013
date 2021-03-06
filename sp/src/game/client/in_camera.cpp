//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//


#include "cbase.h"
#include "hud.h"
#include "kbutton.h"
#include "iinput.h"
#include <vgui/IInput.h>
#include "vgui_controls/Controls.h"
#include "tier0/vprof.h"
#include "debugoverlay_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-------------------------------------------------- Global Variables

static ConVar cam_command( "cam_command", "0", FCVAR_CHEAT | FCVAR_CHEAT);	 // tells camera to go to thirdperson
static ConVar cam_snapto( "cam_snapto", "0", FCVAR_ARCHIVE | FCVAR_CHEAT);	 // snap to thirdperson view
static ConVar cam_ideallag( "cam_ideallag", "4.0", FCVAR_ARCHIVE| FCVAR_CHEAT, "Amount of lag used when matching offset to ideal angles in thirdperson view" );
static ConVar cam_idealdelta( "cam_idealdelta", "4.0", FCVAR_ARCHIVE| FCVAR_CHEAT, "Controls the speed when matching offset to ideal angles in thirdperson view" );
ConVar cam_idealyaw( "cam_idealyaw", "0", FCVAR_ARCHIVE| FCVAR_CHEAT );	 // thirdperson yaw
ConVar cam_idealpitch( "cam_idealpitch", "0", FCVAR_ARCHIVE | FCVAR_CHEAT  );	 // thirperson pitch
ConVar cam_idealdist( "cam_idealdist", "150", FCVAR_ARCHIVE | FCVAR_CHEAT );	 // thirdperson distance
ConVar cam_idealdistright( "cam_idealdistright", "0", FCVAR_ARCHIVE | FCVAR_CHEAT );	 // thirdperson distance
ConVar cam_idealdistup( "cam_idealdistup", "0", FCVAR_ARCHIVE | FCVAR_CHEAT );	 // thirdperson distance
static ConVar cam_collision( "cam_collision", "1", FCVAR_ARCHIVE | FCVAR_CHEAT, "When in thirdperson and cam_collision is set to 1, an attempt is made to keep the camera from passing though walls." );
static ConVar cam_showangles( "cam_showangles", "0", FCVAR_CHEAT, "When in thirdperson, print viewangles/idealangles/cameraoffsets to the console." );
static ConVar c_maxpitch( "c_maxpitch", "90", FCVAR_ARCHIVE| FCVAR_CHEAT );
static ConVar c_minpitch( "c_minpitch", "0", FCVAR_ARCHIVE| FCVAR_CHEAT );
static ConVar c_maxyaw( "c_maxyaw",   "135", FCVAR_ARCHIVE | FCVAR_CHEAT);
static ConVar c_minyaw( "c_minyaw",   "-135", FCVAR_ARCHIVE| FCVAR_CHEAT );
static ConVar c_maxdistance( "c_maxdistance",   "200", FCVAR_ARCHIVE| FCVAR_CHEAT );
static ConVar c_mindistance( "c_mindistance",   "30", FCVAR_ARCHIVE| FCVAR_CHEAT );
static ConVar c_orthowidth( "c_orthowidth",   "100", FCVAR_ARCHIVE| FCVAR_CHEAT );
static ConVar c_orthoheight( "c_orthoheight",   "100", FCVAR_ARCHIVE | FCVAR_CHEAT );

static kbutton_t cam_pitchup, cam_pitchdown, cam_yawleft, cam_yawright;
static kbutton_t cam_in, cam_out; // -- "cam_move" is unused

extern ConVar cl_thirdperson;


// API Wrappers

/*
==============================
CAM_ToThirdPerson

==============================
*/
void CAM_ToThirdPerson(void)
{
}

/*
==============================
CAM_ToThirdPerson_MayaMode

==============================
*/
static bool & Is_CAM_ThirdPerson_MayaMode(void)
{
	static bool s_b_CAM_ThirdPerson_MayaMode = false;
	return s_b_CAM_ThirdPerson_MayaMode;
}
void CAM_ToThirdPerson_MayaMode(void)
{
	bool &rb = Is_CAM_ThirdPerson_MayaMode();
	rb = !rb;
}

/*
==============================
CAM_ToggleSnapto

==============================
*/
void CAM_ToggleSnapto( void )
{ 
	cam_snapto.SetValue( !cam_snapto.GetInt() );
}


/*
==============================
MoveToward

==============================
*/
float MoveToward( float cur, float goal, float lag )
{
	if( cur != goal )
	{
		if( abs( cur - goal ) > 180.0 )
		{
			if( cur < goal )
				cur += 360.0;
			else
				cur -= 360.0;
		}

		if( cur < goal )
		{
			if( cur < goal - 1.0 )
				cur += ( goal - cur ) / lag;
			else
				cur = goal;
		}
		else
		{
			if( cur > goal + 1.0 )
				cur -= ( cur - goal ) / lag;
			else
				cur = goal;
		}
	}


	// bring cur back into range
	if( cur < 0 )
		cur += 360.0;
	else if( cur >= 360 )
		cur -= 360;

	return cur;
}

//------------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------------
void ClampRange180( float &value )
{
	if ( value >= 180.0f )
	{
		value -= 360.0f;
	}
	else if ( value <= -180.0f )
	{
		value += 360.0f;
	}
}

void CAM_PitchUpDown( const CCommand &args ) { KeyDown( &cam_pitchup, args[1] ); }
void CAM_PitchUpUp( const CCommand &args ) { KeyUp( &cam_pitchup, args[1] ); }
void CAM_PitchDownDown( const CCommand &args ) { KeyDown( &cam_pitchdown, args[1] ); }
void CAM_PitchDownUp( const CCommand &args ) { KeyUp( &cam_pitchdown, args[1] ); }
void CAM_YawLeftDown( const CCommand &args ) { KeyDown( &cam_yawleft, args[1] ); }
void CAM_YawLeftUp( const CCommand &args ) { KeyUp( &cam_yawleft, args[1] ); }
void CAM_YawRightDown( const CCommand &args ) { KeyDown( &cam_yawright, args[1] ); }
void CAM_YawRightUp( const CCommand &args ) { KeyUp( &cam_yawright, args[1] ); }
void CAM_InDown( const CCommand &args ) { KeyDown( &cam_in, args[1] ); }
void CAM_InUp( const CCommand &args ) { KeyUp( &cam_in, args[1] ); }
void CAM_OutDown( const CCommand &args ) { KeyDown( &cam_out, args[1] ); }
void CAM_OutUp( const CCommand &args ) { KeyUp( &cam_out, args[1] ); }

static ConCommand startpitchup( "+campitchup", CAM_PitchUpDown );
static ConCommand endpitcup( "-campitchup", CAM_PitchUpUp );
static ConCommand startpitchdown( "+campitchdown", CAM_PitchDownDown );
static ConCommand endpitchdown( "-campitchdown", CAM_PitchDownUp );
static ConCommand startcamyawleft( "+camyawleft", CAM_YawLeftDown );
static ConCommand endcamyawleft( "-camyawleft", CAM_YawLeftUp );
static ConCommand startcamyawright( "+camyawright", CAM_YawRightDown );
static ConCommand endcamyawright( "-camyawright", CAM_YawRightUp );
static ConCommand startcamin( "+camin", CAM_InDown );
static ConCommand endcamin( "-camin", CAM_InUp );
static ConCommand startcamout( "+camout", CAM_OutDown );
static ConCommand camout( "-camout", CAM_OutUp );
static ConCommand thirdperson_mayamode( "thirdperson_mayamode", ::CAM_ToThirdPerson_MayaMode, "Switch to thirdperson Maya-like camera controls.", FCVAR_CHEAT );

// TF allows servers to push people into first/thirdperson, for mods
#ifdef TF_CLIENT_DLL
static ConCommand thirdperson( "thirdperson", ::CAM_ToThirdPerson, "Switch to thirdperson camera.", FCVAR_CHEAT | FCVAR_SERVER_CAN_EXECUTE );
static ConCommand firstperson( "firstperson", ::CAM_ToFirstPerson, "Switch to firstperson camera.", FCVAR_SERVER_CAN_EXECUTE );
#else
/*
static ConCommand thirdperson( "thirdperson", ::CAM_ToThirdPerson, "Switch to thirdperson camera.", FCVAR_CHEAT );
static ConCommand firstperson( "firstperson", ::CAM_ToFirstPerson, "Switch to firstperson camera." );
*/
#endif
static ConCommand snapto( "snapto", CAM_ToggleSnapto );
