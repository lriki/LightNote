//==============================================================================
// �����̍Đ��`���[���A��
//------------------------------------------------------------------------------
/*
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	// ���C�����[�v
	lnBool endRequested;
	do
	{
		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}