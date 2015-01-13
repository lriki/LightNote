//==============================================================================
// Input
//------------------------------------------------------------------------------
/**
	@page modules1_input1 ���͋@�\
	
	- @ref modules1_input1_anchor1		"�L�[�{�[�h�A�}�E�X�A�W���C�p�b�h�A�̓��͂��܂Ƃ߂Ĉ���(���z�R���g���[��)"
	- @ref modules1_input1_anchor2		"���z�R���g���[���̃L�[�R���t�B�O���s��"
	- @ref modules1_input1_anchor3		"�L�[�{�[�h�A�}�E�X�A�W���C�p�b�h�A�̓��͂��ʂɈ���"
	<br>
	<br>
	
	@if DOXY_COMMENT --------------------------------------------------- @endif
	<hr>
	@anchor modules1_input1_anchor1
	@par �L�[�{�[�h�A�W���C�p�b�h�A�}�E�X�̓��͂��܂Ƃ߂Ĉ���(���z�R���g���[��)
	<br>
	@if DOXY_COMMENT --------------------------------------------------- @endif
	��ʓI�ȉƒ�p�Q�[���@�ł̓R���g���[����1��ނŁA�u���{�^���v�͍U���A�u���{�^���v�h��A�̂悤�ɖ��������܂��Ă��܂��B<br>
	������PC�Q�[���ł̓L�[�{�[�h�A�}�E�X�A�W���C�p�b�h�₻��ȊO�̗l�X�ȃf�o�C�X��ڑ��\�ŁA<br>
	�������J�o�[���邽�߂Ƀv���O���}���C���g���̂͑�ςł��B<br>
	<br>
	LNote �ł͗l�X�ȓ��̓f�o�C�X����̓��͂��A�ЂƂ̃��W���[������ȒP�Ɉ������Ƃ��ł��܂��B<br>
	<br>
	�Ⴆ�΁A�u���{�^���v��������Ă��邩���`�F�b�N�������Ƃ��͈ȉ��̂悤�ɂ��܂��B<br>
	�u���{�^���v�Ƃ��ēo�^����Ă���L�[�{�[�h�A�Q�[���p�b�h�A�}�E�X�̑S�ẴL�[�E�{�^�����m�F���A<br>
	�����ꂩ�ЂƂł���������Ă���΁u������Ă���v�Ƃ݂Ȃ��܂��B<br>
	<br>
@ln_codeblock
	@ln_tabdecl
		@ln_tab{tab1,C/C++}
		@ln_tab{tab_csharp,C#}
		@ln_tab{tab_hsp,HSP}
	@ln_endtabdecl
	@ln_code{tab1,cpp}
if ( LInput_IsPress( 0, LN_BUTTON_LEFT ) ) {
	// ���{�^����������Ă���
}

	@ln_endcode
	@ln_code{tab_csharp,csharp}
if ( Input[0].IsPress( Button.Left ) ) {
	// ���{�^����������Ă���
}

	@ln_endcode
	@ln_code{tab_hsp,cpp}
LInput_IsPress 0, LN_BUTTON_LEFT
if stat == LN_TRUE {
	// ���{�^����������Ă���
}
	@ln_endcode
@ln_endcodeblock
	<br>
	���̂悤�ɃL�[�{�[�h��W���C�p�b�h�̋�ʂ������A<br>
	�u���{�^���������̃R���g���[���v�ɒu�������čl���邱�Ƃ��u���z�R���g���[���v�ƌĂ�ł��܂��B<br>
	<br>
	<br>
	���̕\�́A������Ԃ̊��蓖�Ĉꗗ�ł��B<br>
	<br>
	<table>
	<tr>
		<td>���z�{�^����</td>
		<td>�萔��</td>
		<td>�L�[�{�[�h</td>
		<td>�}�E�X</td>
		<td>�W���C�p�b�h</td>
	</tr>
	<tr>
		<td><b>A �{�^��</b></td>	<td>LN_BUTTON_A</td>	<td>'Z' �L�[</td>	<td>-</td>	<td>1 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>B �{�^��</b></td>	<td>LN_BUTTON_B</td>	<td>'X' �L�[</td>	<td>-</td>	<td>2 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>C �{�^��</b></td>	<td>LN_BUTTON_C</td>	<td>'C' �L�[</td>	<td>-</td>	<td>3 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>X �{�^��</b></td>	<td>LN_BUTTON_X</td>	<td>'A' �L�[</td>	<td>-</td>	<td>4 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>Y �{�^��</b></td>	<td>LN_BUTTON_Y</td>	<td>'S' �L�[</td>	<td>-</td>	<td>5 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>Z �{�^��</b></td>	<td>LN_BUTTON_Z</td>	<td>'D' �L�[</td>	<td>-</td>	<td>6 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>L �{�^��</b></td>	<td>LN_BUTTON_L</td>	<td>'Q' �L�[</td>	<td>-</td>	<td>7 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>R �{�^��</b></td>	<td>LN_BUTTON_R</td>	<td>'W' �L�[</td>	<td>-</td>	<td>8 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>L2 �{�^��</b></td>	<td>LN_BUTTON_L2</td>	<td>-</td>			<td>-</td>	<td>9 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>R2 �{�^��</b></td>	<td>LN_BUTTON_R2</td>	<td>-</td>			<td>-</td>	<td>10 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>L3 �{�^��</b></td>	<td>LN_BUTTON_L3</td>	<td>-</td>			<td>-</td>	<td>11 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>R3 �{�^��</b></td>	<td>LN_BUTTON_R3</td>	<td>-</td>			<td>-</td>	<td>12 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>Start �{�^��</b></td>	<td>LN_BUTTON_START</td>	<td>-</td>	<td>-</td>	<td>13 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>Select �{�^��</b></td>	<td>LN_BUTTON_SELECT</td>	<td>-</td>	<td>-</td>	<td>14 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>Sub1 �{�^��</b></td>	<td>LN_BUTTON_SUB1</td>	<td>-</td>			<td>-</td>	<td>15 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>Sub2 �{�^��</b></td>	<td>LN_BUTTON_SUB2</td>	<td>-</td>			<td>-</td>	<td>16 �ԃ{�^��</td>
	</tr>
	<tr>
		<td><b>�� �{�^��</b></td>	<td>LN_BUTTON_LEFT</td>	<td>�A���[�L�[ '��'</td>		<td>-</td>	<td>POV�� / ��1��-</td>
	</tr>
	<tr>
		<td><b>�E �{�^��</b></td>	<td>LN_BUTTON_RIGHT</td><td>�A���[�L�[ '��'</td>		<td>-</td>	<td>POV�E / ��1��+</td>
	</tr>
	<tr>
		<td><b>�� �{�^��</b></td>	<td>LN_BUTTON_UP</td>	<td>�A���[�L�[ '��'</td>		<td>-</td>	<td>POV�� / ��2��-</td>
	</tr>
	<tr>
		<td><b>�� �{�^��</b></td>	<td>LN_BUTTON_DOWN</td>	<td>�A���[�L�[ '��'</td>		<td>-</td>	<td>POV�� / ��2��+</td>
	</tr>
	</table>
	<br>
	�Ⴆ�Ύd�l�ŁuA�{�^���͍U���v�ƌ��߂Ă��܂��΁A<br>
	�v���O���~���O���� LN_BUTTON_A �𗘗p���ĉ���������s���΃X���[�Y�Ɏ����ł��܂��B<br>
	<br>
	�Ȃ��A�����͂����܂Ńf�t�H���g�ŗp�ӂ���Ă���}�b�s���O�ł��B<br>
	��q�̃L�[�R���t�B�O���g�p���āA�㉺���E�̑�����L�[�{�[�h�� W,A,S,D �ōs������A<br>
	�W���C�p�b�h�̃X�e�B�b�N�̏������ A �{�^���Ƃ���E�E�E�A�Ƃ������J�X�^�}�C�Y���\�ł��B<br>
	<br>
	<br>
	
	@if DOXY_COMMENT --------------------------------------------------- @endif
	<hr>
	@anchor modules1_input1_anchor2
	@par ���z�R���g���[���̃L�[�R���t�B�O���s��
	<br>
	@if DOXY_COMMENT --------------------------------------------------- @endif
	���z�R���g���[���̊e�{�^�����ƂɁA�ǂ̃f�o�C�X�̂ǂ̃{�^�������蓖�Ă邩���ׂ����w�肷�邱�Ƃ��ł��܂��B<br>
	<br>
	���̗�ł́A�ȉ��̂悤�Ƀf�o�C�X�̃{�^�������z�R���g���[���̃{�^���Ɋ��蓖�ĂĂ��܂��B<br>
	�E�@�L�[�{�[�h�� W �L�[�� ��{�^���Ƃ���<br>
	�E�@�}�E�X�̍��{�^���� A �{�^���Ƃ���<br>
	�E�@�W���C�p�b�h�̑�3����+������ Z �{�^���Ƃ���<br>
	<br>
@ln_codeblock
	@ln_tabdecl
		@ln_tab{tab1,C/C++}
		@ln_tab{tab_csharp,C#}
		@ln_tab{tab_hsp,HSP}
	@ln_endtabdecl
	@ln_code{tab1,cpp}
LInput_AttachDeviceButton( 0, LN_BUTTON_UP, LN_KEY_W );
LInput_AttachDeviceButton( 0, LN_BUTTON_A,  LN_DEVICEBUTTON_MOUSE_1 );
LInput_AttachDeviceButton( 0, LN_BUTTON_Z,  LN_DEVICEBUTTON_JOYPAD_AXIS_3_PLUS );
	@ln_endcode
	@ln_code{tab_csharp,csharp}
Input[0].AttachDeviceButton( Button.Up, Key.W );
Input[0].AttachDeviceButton( Button.A,  DeviceButton.Mouse1 );
Input[0].AttachDeviceButton( Button.Z,  DeviceButton.JoypadAxis3Plus );
	@ln_endcode
	@ln_code{tab_hsp,cpp}
LInput_AttachDeviceButton 0, LN_BUTTON_UP, LN_KEY_W
LInput_AttachDeviceButton 0, LN_BUTTON_A,  LN_DEVICEBUTTON_MOUSE_1
LInput_AttachDeviceButton 0, LN_BUTTON_Z,  LN_DEVICEBUTTON_JOYPAD_AXIS_3_PLUS
	@ln_endcode
@ln_endcodeblock
	<br>
	�ЂƂ̉��z�{�^���ɂ͕����̃f�o�C�X�{�^�������蓖�Ă邱�Ƃ��ł��܂��B<br>
	��L�̗�ł���΁A�u��{�^���v�ɂ̓f�t�H���g�̐ݒ�ƍ��킹��<br>
	�E�@�A���[�L�[�� '��'<br>
	�E�@'W' �L�[<br>
	��2�����蓖�Ă��Ă��܂��̂ŁA���̂ǂ��炩��������������΁u��{�^���������ꂽ�v�Ƃ݂Ȃ���܂��B<br>
	<br>
	<br>
	���[�U�[�����������f�o�C�X�{�^�����擾���A���z�{�^���Ɋ��蓖�Ă�ɂ͎��̂悤�ɂ��܂��B<br>
	<br>
@ln_codeblock
	@ln_tabdecl
		@ln_tab{tab1,C/C++}
		@ln_tab{tab_csharp,C#}
		@ln_tab{tab_hsp,HSP}
	@ln_endtabdecl
	@ln_code{tab1,cpp}
int deviceButton = LInput_GetPressedDeviceButton( 0 );
LInput_AttachDeviceButton( 0, LN_BUTTON_A,  deviceButton );
	@ln_endcode
	@ln_code{tab_csharp,csharp}
DeviceButton deviceButton = Input[0].GetPressedDeviceButton();
Input[0].AttachDeviceButton( Button.A, deviceButton );
	@ln_endcode
	@ln_code{tab_hsp,cpp}
LInput_GetPressedDeviceButton 0
LInput_AttachDeviceButton 0, LN_BUTTON_A, stat
	@ln_endcode
@ln_endcodeblock
	<br>
	LNote ���F���ł���S�Ẵf�o�C�X�̃L�[�A�{�^���A�����͂���A<br>
	�V���������ꂽ�f�o�C�X�{�^�������蓖�ĂĂ��܂��B<br>
	<br>
	<br>
	
	@if DOXY_COMMENT --------------------------------------------------- @endif
	<hr>
	@anchor modules1_input1_anchor3
	@par �L�[�{�[�h�A�W���C�p�b�h�A�}�E�X�̓��͂��ʂɈ���
	<br>
	@if DOXY_COMMENT --------------------------------------------------- @endif
	<br>
	<br>

	<br>
	<br>
*/
