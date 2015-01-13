//==============================================================================
// 音声の再生チューリアル
//------------------------------------------------------------------------------
/*
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	// メインループ
	lnBool endRequested;
	do
	{
		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}