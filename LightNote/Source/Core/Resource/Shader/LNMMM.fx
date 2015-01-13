//==============================================================================
// MikuMikuMoving
//------------------------------------------------------------------------------
//==============================================================================

#define MIKUMIKUMOVING

static const int MMM_LightCount = 3;

bool	 MMM_LightEnables[MMM_LightCount]	 : LightEnables;

float	 MMM_ShadowDeep			: SHADOWDEEP;
float	 MMM_ShadowDeepPlus		: SHADOWDEEP_POSITIVE;
float	 MMM_ShadowDeepMinus	: SHADOWDEEP_NEGATIVE;

bool	 MMM_usetoon			: MMM_USETOON;
float	 MMM_modelsize			: MMM_MODELSIZE;

bool	 MMM_IsDinamicProjection: MMM_ISDYNAMICFOV;

static const float MMM_Epsilon = 0.004f;
