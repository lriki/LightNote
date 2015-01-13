//==============================================================================
// Input
//------------------------------------------------------------------------------
/**
	@page modules1_input1 入力機能
	
	- @ref modules1_input1_anchor1		"キーボード、マウス、ジョイパッド、の入力をまとめて扱う(仮想コントローラ)"
	- @ref modules1_input1_anchor2		"仮想コントローラのキーコンフィグを行う"
	- @ref modules1_input1_anchor3		"キーボード、マウス、ジョイパッド、の入力を個別に扱う"
	<br>
	<br>
	
	@if DOXY_COMMENT --------------------------------------------------- @endif
	<hr>
	@anchor modules1_input1_anchor1
	@par キーボード、ジョイパッド、マウスの入力をまとめて扱う(仮想コントローラ)
	<br>
	@if DOXY_COMMENT --------------------------------------------------- @endif
	一般的な家庭用ゲーム機ではコントローラが1種類で、「○ボタン」は攻撃、「□ボタン」防御、のように役割が決まっています。<br>
	しかしPCゲームではキーボード、マウス、ジョイパッドやそれ以外の様々なデバイスを接続可能で、<br>
	これらをカバーするためにプログラマが気を使うのは大変です。<br>
	<br>
	LNote では様々な入力デバイスからの入力を、ひとつのモジュールから簡単に扱うことができます。<br>
	<br>
	例えば、「左ボタン」が押されているかをチェックしたいときは以下のようにします。<br>
	「左ボタン」として登録されているキーボード、ゲームパッド、マウスの全てのキー・ボタンを確認し、<br>
	いずれかひとつでも押下されていれば「押されている」とみなします。<br>
	<br>
@ln_codeblock
	@ln_tabdecl
		@ln_tab{tab1,C/C++}
		@ln_tab{tab_csharp,C#}
		@ln_tab{tab_hsp,HSP}
	@ln_endtabdecl
	@ln_code{tab1,cpp}
if ( LInput_IsPress( 0, LN_BUTTON_LEFT ) ) {
	// 左ボタンが押されている
}

	@ln_endcode
	@ln_code{tab_csharp,csharp}
if ( Input[0].IsPress( Button.Left ) ) {
	// 左ボタンが押されている
}

	@ln_endcode
	@ln_code{tab_hsp,cpp}
LInput_IsPress 0, LN_BUTTON_LEFT
if stat == LN_TRUE {
	// 左ボタンが押されている
}
	@ln_endcode
@ln_endcodeblock
	<br>
	このようにキーボードやジョイパッドの区別をせず、<br>
	「左ボタンを持つ仮のコントローラ」に置き換えて考えることを「仮想コントローラ」と呼んでいます。<br>
	<br>
	<br>
	次の表は、初期状態の割り当て一覧です。<br>
	<br>
	<table>
	<tr>
		<td>仮想ボタン名</td>
		<td>定数名</td>
		<td>キーボード</td>
		<td>マウス</td>
		<td>ジョイパッド</td>
	</tr>
	<tr>
		<td><b>A ボタン</b></td>	<td>LN_BUTTON_A</td>	<td>'Z' キー</td>	<td>-</td>	<td>1 番ボタン</td>
	</tr>
	<tr>
		<td><b>B ボタン</b></td>	<td>LN_BUTTON_B</td>	<td>'X' キー</td>	<td>-</td>	<td>2 番ボタン</td>
	</tr>
	<tr>
		<td><b>C ボタン</b></td>	<td>LN_BUTTON_C</td>	<td>'C' キー</td>	<td>-</td>	<td>3 番ボタン</td>
	</tr>
	<tr>
		<td><b>X ボタン</b></td>	<td>LN_BUTTON_X</td>	<td>'A' キー</td>	<td>-</td>	<td>4 番ボタン</td>
	</tr>
	<tr>
		<td><b>Y ボタン</b></td>	<td>LN_BUTTON_Y</td>	<td>'S' キー</td>	<td>-</td>	<td>5 番ボタン</td>
	</tr>
	<tr>
		<td><b>Z ボタン</b></td>	<td>LN_BUTTON_Z</td>	<td>'D' キー</td>	<td>-</td>	<td>6 番ボタン</td>
	</tr>
	<tr>
		<td><b>L ボタン</b></td>	<td>LN_BUTTON_L</td>	<td>'Q' キー</td>	<td>-</td>	<td>7 番ボタン</td>
	</tr>
	<tr>
		<td><b>R ボタン</b></td>	<td>LN_BUTTON_R</td>	<td>'W' キー</td>	<td>-</td>	<td>8 番ボタン</td>
	</tr>
	<tr>
		<td><b>L2 ボタン</b></td>	<td>LN_BUTTON_L2</td>	<td>-</td>			<td>-</td>	<td>9 番ボタン</td>
	</tr>
	<tr>
		<td><b>R2 ボタン</b></td>	<td>LN_BUTTON_R2</td>	<td>-</td>			<td>-</td>	<td>10 番ボタン</td>
	</tr>
	<tr>
		<td><b>L3 ボタン</b></td>	<td>LN_BUTTON_L3</td>	<td>-</td>			<td>-</td>	<td>11 番ボタン</td>
	</tr>
	<tr>
		<td><b>R3 ボタン</b></td>	<td>LN_BUTTON_R3</td>	<td>-</td>			<td>-</td>	<td>12 番ボタン</td>
	</tr>
	<tr>
		<td><b>Start ボタン</b></td>	<td>LN_BUTTON_START</td>	<td>-</td>	<td>-</td>	<td>13 番ボタン</td>
	</tr>
	<tr>
		<td><b>Select ボタン</b></td>	<td>LN_BUTTON_SELECT</td>	<td>-</td>	<td>-</td>	<td>14 番ボタン</td>
	</tr>
	<tr>
		<td><b>Sub1 ボタン</b></td>	<td>LN_BUTTON_SUB1</td>	<td>-</td>			<td>-</td>	<td>15 番ボタン</td>
	</tr>
	<tr>
		<td><b>Sub2 ボタン</b></td>	<td>LN_BUTTON_SUB2</td>	<td>-</td>			<td>-</td>	<td>16 番ボタン</td>
	</tr>
	<tr>
		<td><b>左 ボタン</b></td>	<td>LN_BUTTON_LEFT</td>	<td>アローキー '←'</td>		<td>-</td>	<td>POV左 / 第1軸-</td>
	</tr>
	<tr>
		<td><b>右 ボタン</b></td>	<td>LN_BUTTON_RIGHT</td><td>アローキー '→'</td>		<td>-</td>	<td>POV右 / 第1軸+</td>
	</tr>
	<tr>
		<td><b>上 ボタン</b></td>	<td>LN_BUTTON_UP</td>	<td>アローキー '↑'</td>		<td>-</td>	<td>POV上 / 第2軸-</td>
	</tr>
	<tr>
		<td><b>下 ボタン</b></td>	<td>LN_BUTTON_DOWN</td>	<td>アローキー '↓'</td>		<td>-</td>	<td>POV下 / 第2軸+</td>
	</tr>
	</table>
	<br>
	例えば仕様で「Aボタンは攻撃」と決めてしまえば、<br>
	プログラミング時は LN_BUTTON_A を利用して押下判定を行えばスムーズに実装できます。<br>
	<br>
	なお、これらはあくまでデフォルトで用意されているマッピングです。<br>
	後述のキーコンフィグを使用して、上下左右の操作をキーボードの W,A,S,D で行ったり、<br>
	ジョイパッドのスティックの上方向を A ボタンとする・・・、といったカスタマイズも可能です。<br>
	<br>
	<br>
	
	@if DOXY_COMMENT --------------------------------------------------- @endif
	<hr>
	@anchor modules1_input1_anchor2
	@par 仮想コントローラのキーコンフィグを行う
	<br>
	@if DOXY_COMMENT --------------------------------------------------- @endif
	仮想コントローラの各ボタンごとに、どのデバイスのどのボタンを割り当てるかを細かく指定することができます。<br>
	<br>
	次の例では、以下のようにデバイスのボタンを仮想コントローラのボタンに割り当てています。<br>
	・　キーボードの W キーを 上ボタンとする<br>
	・　マウスの左ボタンを A ボタンとする<br>
	・　ジョイパッドの第3軸の+方向を Z ボタンとする<br>
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
	ひとつの仮想ボタンには複数のデバイスボタンを割り当てることができます。<br>
	上記の例であれば、「上ボタン」にはデフォルトの設定と合わせて<br>
	・　アローキーの '↑'<br>
	・　'W' キー<br>
	の2つが割り当てられていますので、そのどちらか一方が押下されれば「上ボタンが押された」とみなされます。<br>
	<br>
	<br>
	ユーザーが押下したデバイスボタンを取得し、仮想ボタンに割り当てるには次のようにします。<br>
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
	LNote が認識できる全てのデバイスのキー、ボタン、軸入力から、<br>
	新しく押されたデバイスボタンを割り当てています。<br>
	<br>
	<br>
	
	@if DOXY_COMMENT --------------------------------------------------- @endif
	<hr>
	@anchor modules1_input1_anchor3
	@par キーボード、ジョイパッド、マウスの入力を個別に扱う
	<br>
	@if DOXY_COMMENT --------------------------------------------------- @endif
	<br>
	<br>

	<br>
	<br>
*/
