#ifndef MOUSE_MANAGER_H
#define MOUSE_MANAGER_H

// Mouse system parameters.
enum {
		MOUSE_ACCEL_THRESHOLD_1 = 0, // Mickey movement threshold after which to double.
		MOUSE_ACCEL_THRESHOLD_2 = 1, // Mickey movement threshold after which to double again.
		MOUSE_SPEED_FACTOR = 2,	// 0 = no acceleration, 1 = first threshold enabled, 2 = both enabled.
		NUM_MOUSE_PARAMS = 3,
};

// Class that manages mouse movement.
class CMouseManager
{

public:

	CMouseManager( void );
	~CMouseManager( void );

	// Initialize mouse.
	void Initialize( void );

	// Mouse enable/disable.
	void ActivateMouse( void );
	void DeactivateMouse( void );

	// Mouse movement functions.
	void UpdateMouse( float *deltaX, float *deltaY );

	// Mouse cursor position functions.
	void GetCursorPosition( int *pX, int *pY );
	void SetCursorPosition( int x, int y );
	void ResetToWindowCenter( void );

private:

	// Mouse movement and accumulation.
	void AccumulateMovement( void );
	void GetAccumulatedMovement( float *pX, float *pY );
	void ResetAccumulatedMovement( void );
	void FilterMouseDelta( float *pX, float *pY );
	void ScaleMouseDelta( float *pX, float *pY );

	// Apply mouse acceleration.
	void CheckAccelerationOverride( void );
	void ApplyAccelerationSettings( void );
	void SaveAccelerationSettings( void );
	void RestoreAccelerationSettings( void );

	// Helper for getting screen center.
	void GetWindowSize( int *pWidth, int *pHeight );
	void GetWindowCenter( int *pX, int *pY );

private:

	// Mouse manager initialization and activation state.
	bool m_bInitialized;
	bool m_bActive;

	// Mouse system parameters.
	bool m_bHasSavedParameters;
	int m_oldParameters[NUM_MOUSE_PARAMS];
	int m_newParameters[NUM_MOUSE_PARAMS];
	bool m_bUseConsoleVariable[NUM_MOUSE_PARAMS];

	// Previous mouse accumulation sample for filtering.
	float m_fPreviousSampleX;
	float m_fPreviousSampleY;

	// Current accumulated movement.
	int m_iAccumulatedX;
	int m_iAccumulatedY;

};

#endif // MOUSE_MANAGER_H