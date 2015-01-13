//=============================================================================
// �v���O���~���O�K�C�h - ������
//-----------------------------------------------------------------------------
/**
    @page modules1_initialize1 LightNote �̏�����

    - @ref modules1_initialize1_anchor1   "�S�Ă̋@�\�����������A���s����"
    - @ref modules1_initialize1_anchor2   "�����ݒ���s��"
    - @ref modules1_initialize1_anchor3   "�K�v�ȋ@�\�����ʂɏ���������"
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_initialize1_anchor1
    @par �S�Ă̋@�\�����������A���s����
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    LightNote �𐳂������삳����ŏ����̃v���O�����͎��̂Ƃ���ł��B<br>
    ���̃v���O���������s����Ƌ�̃E�B���h�E���\������A�N���[�Y�{�^���������ƃv���O�������I�����܂��B<br>
    <br>
    <LN_CodeBlock>
    <LN_Code C>
#include &lt;lnote.h&gt;

int main()
{
	lnBool isEnd;
	
	// LightNote �̏�����
	LCore_Initialize();

	// ���C�����[�v
	do
	{
		// �����ɃQ�[���̃��C�������������܂�
		
		LCore_Update();
		LCore_IsEndRequested(&isEnd);
	} while(!isEnd);

	// LightNote �̏I������
	LCore_Terminate();
	
	return 0;
}
    </LN_Code>
    <LN_Code C++>
#include &lt;lnote.hpp&gt;
using namespace LN;

int main()
{
	// LightNote �̏�����
	Core::Initialize();
	
	// ���C�����[�v
	do
	{
		// �����ɃQ�[���̃��C�������������܂�
		
		Core::Update();
	} while(!Core::IsEndRequested());
	
	// LightNote �̏I������
	Core::Terminate();
	
	return 0;
}
    </LN_Code>
    <LN_Code C#>
using LN;

class Program
{
	static void Main()
	{
		// LightNote �̏�����
		Core.Initialize();

		// ���C�����[�v
		do
		{
			// �����ɃQ�[���̃��C�������������܂�
			
			Core.Update();
		} while(Core.IsEndRequested);

		// LightNote �̏I������
		Core.Terminate();
	}
}
    </LN_Code>
    <LN_Code Ruby>
require 'lnote'
include LN

# LightNote �̏�����
Core.initialize

# ���C�����[�v
begin
	# �����ɃQ�[���̃��C�������������܂�
	
	Core.update
end while Core.end_requested?

# LightNote �̏I������
Core.terminate
    </LN_Code>
    <LN_Code HSP>
#include "lnote.as"
	// LightNote �̏�����
	LCore_Initialize
	
	// ���C�����[�v
*main
	// �����ɃQ�[���̃��C�������������܂�
		
	LCore_Update
	if (stat == LN_FALSE) : goto *exit
	goto *main
	
*exit
	// LightNote �̏I������
	LCore_Terminate
    </LN_Code>
    </LN_CodeBlock>
    <br>
	�ŏ��� Initialize �̓E�B���h�E�̐�����`��@�\�̏��������s���ALightNote ���g�p�ł����Ԃɂ��܂��B<br>
	�ʏ�A�v���O�����̐擪��1�x�����Ăяo���܂��B<br>
	<br>
	@ref modules1_initialize1_anchor2 "�����ݒ�" ����ɍs��Ȃ���΁A640*480�̃E�B���h�E���쐬�� 60fps �ŉ�ʍX�V���s���悤�ɂȂ�܂��B<br>
	<br>
	<br>
	���̃��C�����[�v�� Update �ɂ��A1 �t���[�����Ƃ̉�ʃN���A�E�`��A���͏�ԓ��̍X�V���s���܂��B<br>
	�܂��A1 ���[�v 60fps �ɋ߂Â���悤�Ɏ����I�ɑ҂����Ԃ����܂��B<br>
	<br>
	IsEndRequested �́A�E�B���h�E�̃N���[�Y�{�^���������ꂽ���A�A�v���P�[�V�������I������ׂ����� true ��Ԃ��܂��B<br>
	���̃v���O�����ł͂��̏����ŁA���C�����[�v���I�������Ă��܂��B<br>
	<br>
	<br>
	�Ō�� Terminate �ŁALightNote �̃��\�[�X�����ׂĉ�����A�g�p���I�����܂��B<br>
	<br>
	<br>
	
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor	modules1_initialize1_anchor2
    @par	�����ݒ���s��
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
	<br>
	<br>
	
	@if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_initialize1_anchor3
    @par	�K�v�ȋ@�\�����ʂɏ���������
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
	<br>
	<br>
	
*/
//=============================================================================