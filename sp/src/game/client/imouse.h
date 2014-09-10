#ifndef IMOUSE_H
#define IMOUSE_H

/* Interface for accumulating and retrieving mouse movement data. */
class IMouse
{

public:

	// Initialize the mouse handler.
	virtual void Initialize( void ) = 0;

	// Activating and deactivating the mouse accumulation and settings.
	virtual void Activate( void ) = 0;
	virtual void Deactivate( void ) = 0;
	virtual bool IsActive( void ) const = 0;

	// Updating and retrieving mouse movement data.
	virtual void AccumulateMovement( void ) = 0;
	virtual void GetMouseMovement( float *deltaX, float *deltaY ) = 0;
	virtual void ResetAccumulatedMovement( void ) = 0;

	// Mouse cursor position functions.
	virtual void GetCursorPosition( int *pX, int *pY ) = 0;
	virtual void SetCursorPosition( int x, int y ) = 0;
	virtual void ResetToWindowCenter( void ) = 0;

};

#endif // IMOUSE_H